#include "FormData.hpp"

FormData::FormData(Request &req) : _split(NULL), _req(NULL)
{
	this->_req = new Request(req);
	try
	{
		this->checkContentType();
		// std::string dummy = this->mapToJason();
	}
	catch (const BadRequest &e)
	{
		this->_req->_statusCode = 400;
		std::cout << e.what() << std::endl;
	}
}

FormData::FormData(FormData const &src)
{
	*this = src;
}

FormData &FormData::operator=(FormData const &src)
{
	if (this != &src)
	{
		if (this->_req != NULL)
			delete this->_req;
		this->_req = new Request(*src._req);
		if (this->_split != NULL)
			delete[] this->_split;
		for (int i = 0; !src._split[i].empty(); ++i)
			this->_split[i] = src._split[i];
	}
	return (*this);
}

FormData::~FormData()
{
	if (this->_req != NULL)
	{
		delete this->_req;
		this->_req = NULL;
	}
	if (this->_split != NULL)
	{
		delete[] this->_split;
		this->_split = NULL;
	}
}

const char *FormData::BadRequest::what() const throw()
{
	return ("400 Bad request");
}

void FormData::printForm()
{
	for (std::vector<std::map<std::string, std::string>>::iterator vec = this->_form.begin(); vec != this->_form.end(); ++vec)
	{
		std::cout << "/////////// Map ////////////" << std::endl;
		this->_req->printMap(*vec);
		std::cout << "////////////////////////////\n"
				  << std::endl;
	}
}

int FormData::ft_strlen_size_t(std::string &str)
{
	size_t i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

std::map<std::string, std::string> FormData::parseChunk(std::string &str)
{
	std::vector<std::string> chunk = this->splitCRLF(str);
	std::vector<std::string>::iterator i = chunk.begin();
	std::map<std::string, std::string> map;
	while (i != chunk.end())
	{
		if (i == chunk.begin())
		{
			this->_split = ft_split(*i, ';');
			if (this->_split == NULL)
				throw BadRequest();
			for (int i = 0; !this->_split[i].empty(); ++i)
			{
				if (i == 0)
				{
					trimHead(this->_split[i], ' ');
					this->parseHeader(map, this->_split[i]);
				}
				else
				{
					std::string *tmp_split = ft_split(this->_split[i], '=');
					if (tmp_split == NULL || !tmp_split[2].empty())
					{
						delete[] tmp_split;
						throw BadRequest();
					}
					trimHeadTail(tmp_split[0], ' ');
					trimHeadTail(tmp_split[1], '\"');
					if (map.find(tmp_split[0]) == map.end())
						map[tmp_split[0]] = tmp_split[1];
					delete[] tmp_split;
					tmp_split = NULL;
				}
			}
			delete[] this->_split;
			this->_split = NULL;
			if (map.find("Content-Disposition") == map.end() || map.find("name") == map.end())
				throw BadRequest();
		}
		else
		{
			this->parseHeader(map, *i);
		}
		++i;
		std::vector<std::string>::iterator tmp = i + 1;
		if (tmp == chunk.end())
			break;
	}
	if (map.find("body") == map.end())
		map["body"] = chunk.back();
	std::cout << map["body"] << std::endl;
	return (map);
}

void FormData::getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound)
{
	start = this->_req->_body.find("\r\n", start);
	std::string::size_type end;
	if (start == std::string::npos)
		return;
	start += 2;
	end = this->_req->_body.find(bound, start);
	std::string::size_type tmp_size = end;
	if (end == std::string::npos)
	{
		start = end;
		return;
	}
	end = this->_req->_body.find_last_of("\r\n", end);
	if (end == std::string::npos)
	{
		start = end;
		return;
	}
	std::string dummy = this->_req->_body.substr(start, end - start);
	trimTail_str(dummy, "\r\n");
	this->_form.push_back(this->parseChunk(dummy));
	chunk.push_back(dummy);
	start = end;
	start = this->_req->_body.find_first_of(bound, start);
}

void FormData::parseHeader(std::map<std::string, std::string> &map, std::string &line)
{
	std::string tmp[2];
	size_t end = line.find_first_of(':');
	if (end == std::string::npos)
		throw BadRequest();
	tmp[0] = line.substr(0, end);
	if (++end == std::string::npos)
		tmp[1] = "";
	else
		tmp[1] = line.substr(end);
	if (tmp == NULL || tmp[0] == "" || tmp[0][0] == ' ' || tmp[0][0] == '\t' || tmp[0].back() == ' ' || tmp[0].back() == '\t' || (tmp[1][0] != ' ' && tmp[1][0] != '\t'))
		throw BadRequest();
	if (std::isalpha(tmp[0][0]) && (tmp[0][0] < 'A' || tmp[0][0] > 'Z'))
		tmp[0][0] -= 32;

	for (int i = 1; tmp[0][i] != '\0'; ++i)
	{
		if (std::isalpha(tmp[0][i]) && (tmp[0][i] < 'a' || tmp[0][i] > 'z'))
			tmp[0][i] += 32;
	}
	std::string::size_type find = tmp[0].find_first_of('-');
	if (find != std::string::npos)
	{
		find += 1;
		if (std::isalpha(tmp[0][find]) && (tmp[0][find] < 'A' || tmp[0][find] > 'Z'))
			tmp[0][find] -= 32;
	}
	if (map.find(tmp[0]) != map.end())
		throw BadRequest();
	std::string::size_type start = 0;
	start = skipChar(tmp[1], start, ' ');
	start = skipChar(tmp[1], start, '\t');
	start = skipChar(tmp[1], start, ' ');
	if (tmp[1][start] == '\0')
		map[tmp[0]] = "";
	else
		map[tmp[0]] = tmp[1].substr(start);
}

void FormData::checkContentType(void)
{
	if (this->_req->_header.find("Content-Type") == this->_req->_header.end())
		return;
	std::string tmp = this->_req->_header["Content-Type"];
	std::string *mime = ft_split(tmp, ';');
	if (mime == NULL)
		return;
	std::string::size_type start = mime[1].find("=");
	std::string::size_type end = 0;
	if (mime[0] != "multipart/form-data" || start == std::string::npos)
	{
		delete[] mime;
		return;
	}
	std::string bound = mime[1].substr(++start);
	if (bound[0] != '-' && bound[1] != '-')
	{
		delete[] mime;
		throw BadRequest();
	}
	delete[] mime;
	std::string::size_type tmp_size = this->_req->_body.find_last_of(bound);
	tmp_size -= (ft_strlen(bound) + 2);
	tmp = this->_req->_body.substr(tmp_size);
	trimTail_str(tmp, "\r\n");
	if (tmp[ft_strlen(tmp) - 2] != '-' || tmp[ft_strlen(tmp) - 3] != '-')
		throw BadRequest();
	start = this->_req->_body.find(bound);
	if (start == std::string::npos)
		return;
	std::vector<std::string> chunk;
	while (start != std::string::npos)
		this->getFromBound(chunk, start, bound);
	this->printForm();
}

std::vector<std::string> FormData::splitCRLF(std::string &buffer)
{
	std::vector<std::string> res;

	std::string::size_type start = 0;
	std::string::size_type end = this->_req->findCLRF(buffer, start);
	while (end != std::string::npos && !this->_req->endLine(buffer, end))
	{
		if (buffer[end] == '\r' && buffer[end + 1] == '\n')
		{
			if (start != end)
				res.push_back(buffer.substr(start, end - start));
			end += 2;
			start = end;
			end = this->_req->findCLRF(buffer, start);
		}
	}
	if (buffer[end] == '\r' && buffer[end + 1] == '\n' && start != end)
	{
		res.push_back(buffer.substr(start, end - start));
		start = end + 2;
	}
	int buff_size = 0;
	for (std::vector<std::string>::iterator it = res.begin(); it != res.end(); ++it)
		buff_size += 1;
	while (buffer[start] != '\0' && (buffer[start] == '\n' || buffer[start] == '\r'))
		++start;
	if (buffer[start] != '\0')
		res.push_back(buffer.substr(start));
	return (res);
}

std::string FormData::getPureBody(std::string &body)
{
	if (this->_form.empty() || this->_form[0].find("body") == this->_form[0].end())
		return (body);
	return (this->_form[0]["body"]);
}

std::string FormData::getFileName(std::string &filename)
{
	if (this->_form.empty() || this->_form[0].find("filename") == this->_form[0].end())
		return (filename);
	return (this->_form[0]["filename"]);
}

// std::string FormData::mapToJason(void)
// {
// 	std::string serializedData = "{";

// 	for (size_t i = 0; i < this->_form.size(); ++i)
// 	{
// 		serializedData += "\"entry" + std::to_string(i) + "\": {";

// 		const std::map<std::string, std::string> &entry = this->_form[i];
// 		bool firstField = true;

// 		for (std::map<std::string, std::string>::const_iterator it = entry.begin(); it != entry.end(); ++it)
// 		{
// 			if (!firstField)
// 				serializedData += ",";

// 			serializedData += "\"" + it->first + "\": \"" + it->second + "\"";
// 			firstField = false;
// 		}

// 		serializedData += "}";

// 		if (i < this->_form.size() - 1)
// 			serializedData += ",";
// 	}

// 	serializedData += "}";
// 	std::cout << "Try print jason format" << std::endl;
// 	std::cout << serializedData << std::endl;
// 	return (serializedData);
// }
