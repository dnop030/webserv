#include "FormData.hpp"

FormData::FormData(Request &req)
{
	this->_req = new Request(req);
	try
	{
		this->checkContentType();
		// req.checkContentType();
	}
	catch (const BadRequest &e)
	{
		this->_req->_statusCode = 400;
		std::cout << e.what() << std::endl;
	}
}

FormData::~FormData()
{
	if (this->_req != NULL)
	{
		delete this->_req;
		this->_req = NULL;
	}
}

const char *FormData::BadRequest::what() const throw()
{
	return ("400 Bad request");
}

int FormData::ft_strlen_size_t(std::string &str)
{
	size_t i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

void FormData::trimTail_str(std::string &str, std::string delim)
{
	std::string res;

	std::string::size_type end = str.find_last_of(delim);
	while (end == std::string::npos)
	{
		end -= this->_req->ft_strlen(delim);
		end = str.find_last_of(delim);
	}
	res = str.substr(0, end - 0);
	str = res;
}

void FormData::trimHead(std::string &str, char delim)
{
	std::string::size_type start = this->_req->skipChar(str, 0, ' ');
	str = str.substr(start);
}

void FormData::parseToMap(void)
{
}

void FormData::getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound)
{
	start = this->_req->_body.find("\r\n", start);
	std::string::size_type end;
	if (start == std::string::npos)
		return;
	start += 2;
	end = this->_req->_body.find(bound, start);
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
	this->trimTail_str(dummy, "\r\n");
	chunk.push_back(dummy);
	start = end;
	start = this->_req->_body.find_first_of(bound, start);
}

void FormData::checkContentType(void)
{
	if (this->_req->_header.find("Content-Type") == this->_req->_header.end())
		return;
	std::string tmp = this->_req->_header["Content-Type"];
	std::string *mime = this->_req->ft_split(tmp, ';');
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
	delete[] mime;
	start = this->_req->_body.find(bound);
	if (start == std::string::npos)
		return;
	std::vector<std::string> chunk;
	while (start != std::string::npos)
	{
		// std::cout << "xxxxxxx try escape delim => new one " << i << std::endl;
		this->getFromBound(chunk, start, bound);
		// if (start != std::string::npos)
		// 	std::cout << chunk.back() << std::endl;
	}
	int i = 1;
	for (std::vector<std::string>::iterator it = chunk.begin(); it != chunk.end(); ++it)
	{
		std::cout << "zzzzzzzz try escape delim => new one " << i << std::endl;
		std::cout << *it << std::endl;
		std::vector<std::string> dummy = this->splitCRLF(*it);
		std::cout << "LLLLLLLLLL Start in splitCRLF LLLLLLLLLL" << std::endl;
		std::vector<std::string>::iterator i = dummy.begin();
		while (i != dummy.end())
		{
			if (i == dummy.begin())
			{
				std::string *firstLine = this->_req->ft_split(*i, ';');
				if (firstLine == NULL)
					throw BadRequest();
				for (int i = 0; !firstLine[i].empty(); ++i)
				{
					// std::string::size_type start = this->_req->skipChar(firstLine[i], 0, ' ');
					// firstLine[i] = firstLine[i].substr(start);
					this->trimHead(firstLine[i], ' ');
					std::cout << "firstLine[" << i << "]: " << firstLine[i] << std::endl;
				}
				delete[] firstLine;
			}
			// std::cout << "-------- Bound --------" << std::endl;
			std::cout << *i << std::endl;
			++i;
			std::vector<std::string>::iterator tmp = i + 1;
			if (tmp == dummy.end())
				break;
		}
		std::cout << "Body is as below" << std::endl;
		std::cout << dummy.back() << std::endl;
		std::cout << "LLLLLLLLLL End in splitCRLF LLLLLLLLLL" << std::endl;
		++i;
	}
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
	{
		res.push_back(buffer.substr(start));
		// 	std::string body = buffer.substr(start);
		// 	std::cout << "///////// Content of each multipart ///////////" << std::endl;
		// 	std::cout << body << std::endl;
	}
	return (res);
}
