#include "Request.hpp"

Request::Request(std::string &buffer) : _buffer(buffer), _line(0), _buff_size(0), _method("")
{
}

Request::Request(Request const &src) : _buff_size(src._buff_size), _buffer(src._buffer), _method(src._method)
{
	*this = src;
}

Request &Request::operator=(Request const &src)
{
	if (this != &src)
	{
		for (int i = 0; i < src._buff_size; ++i)
			this->_line[i] = src._line[i];
	}
	return (*this);
}

Request::~Request()
{
}

const char *Request::BadRequest::what() const throw()
{
	return ("400 Bad request");
}

const char *Request::HttpNotSupport::what() const throw()
{
	return ("505 HTTP Version Not Supported");
}

bool Request::endLine(std::string::size_type idx)
{
	if (this->_buffer[idx] == '\n' && this->_buffer[idx + 1] != '\0')
	{
		if (this->_buffer[idx + 1] == '\n')
			return (true);
		if (this->_buffer[idx + 1] == '\r' && this->_buffer[idx + 2] != '\0' && this->_buffer[idx + 2] == '\n')
			return (true);
	}
	return (false);
}

int Request::countChar(std::string &str, char c)
{
	int res = 0;
	for (std::string::size_type idx = 0; str[idx] != '\0'; ++idx)
	{
		if (str[idx] == c)
			++res;
	}
	return (res);
}

void Request::removeChar(std::string &str, char c)
{
	std::string res;
	for (std::string::size_type idx = 0; str[idx] != '\0'; ++idx)
	{
		if (str[idx] != c)
			res += str[idx];
	}
	str = res;
}

int Request::countReturn(std::string &str)
{
	int res = 0;
	for (std::string::size_type idx = str.length(); idx > 0; --idx)
	{
		if (str[idx - 1] == '\r')
			++res;
		else
			break;
	}
	return (res);
}

void Request::trimTail(std::string &str, char delim)
{
	std::string res;

	std::string::size_type end = str.length();
	while (end > 0 && str[end - 1] == delim)
		--end;
	for (std::string::size_type idx = 0; idx != end; ++idx)
		res += str[idx];
	str = res;
}

std::vector<std::string> Request::splitLine(void)
{
	std::vector<std::string> res;
	int size = 0;

	for (std::string::size_type idx = 0; this->_buffer[idx] != '\0'; ++idx)
	{
		if (this->_buffer[idx] == '\n')
		{
			++size;
			if (this->endLine(idx))
				break;
		}
	}
	std::cout << "size is " << size << std::endl;
	if (size <= 2)
	{
		throw BadRequest();
	}
	std::cout << "Size " << size - 1 << std::endl;
	this->_buff_size = --size;
	int i = 0;
	std::string::size_type start = 0;
	std::string::size_type end = this->_buffer.find('\n');
	while (end != std::string::npos && i < size)
	{
		if (this->_buffer[end] == '\n')
		{
			res.push_back(this->_buffer.substr(start, end - start));
			start = end + 1;
			end = this->_buffer.find('\n', start);
		}
	}
	if (this->_buffer[start] != '\0')
		this->_body = this->_buffer.substr(start);
	return (res);
}

std::string::size_type Request::skipChar(std::string &str, std::string::size_type idx, char c)
{
	while (str[idx] == c && str[idx] != '\0')
		++idx;
	return (idx);
}

int Request::ft_stoi(const std::string &s)
{
	int res = 0;

	std::istringstream iss(s);
	if (!(iss >> res))
		return (0);
	return (res);
}

size_t Request::ft_strlen(std::string &str)
{
	size_t i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

int Request::allDigit(std::string &str)
{
	for (int i = 0; str[i] != '\0'; ++i)
	{
		if (!std::isdigit(str[i]))
			return (0);
	}
	return (1);
}

int Request::ft_strncmp(std::string &str1, std::string &str2, size_t size)
{
	for (size_t i = 0; i < size && str1[i] != '\0' && str2[i] != '\0'; ++i)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
	}
	return (0);
}

void Request::checkHttpVer(std::string &str)
{
	std::string tmp = "HTTP";
	std::string *buffslash = this->ft_split(str, '/');
	if (buffslash == NULL || this->ft_strncmp(buffslash[0], tmp, this->ft_strlen(tmp) != 0))
	{
		delete[] buffslash;
		throw BadRequest();
	}
	std::string *buffdot = this->ft_split(buffslash[1], '.');
	delete[] buffslash;
	buffslash = NULL;
	if (buffdot == NULL)
		throw BadRequest();
	int size = 0;
	while (!buffdot[size].empty())
		++size;
	if (size != 2 || !this->allDigit(buffdot[0]) || !std::isdigit(buffdot[1][0]))
	{
		delete[] buffdot;
		throw BadRequest();
	}
	tmp = "HTTP/1.1";
	if (this->ft_strncmp(str, tmp, this->ft_strlen(tmp)) != 0)
	{
		delete[] buffdot;
		throw HttpNotSupport();
	}
	if (!this->allDigit(buffdot[1]))
	{
		delete[] buffdot;
		throw BadRequest();
	}
}

void Request::checkMethod(std::string &str)
{
	if (str[0] == ' ' || str[0] == '\t')
		throw BadRequest();
	if (str == "GET")
		this->_method = "GET";
	else if (str == "POST")
		this->_method = "POST";
	else if (str == "DELETE")
		this->_method = "DELETE";
	else
		this->_method = "";
}

void Request::checkTargetUri(std::string &str)
{
	if (str[0] == '/')
	{
		std::cout << "go to reconstruct uri" << std::endl;
		return;
	}
	std::string tmp = str;
	for (int i = 0; i < 4; ++i)
	{
		if (std::isalpha(tmp[i]) == 0)
			throw BadRequest();
		if (tmp[i] >= 'A' && tmp[i] <= 'Z')
			tmp[i] += 32;
	}
	std::string http = "http://";
	if (this->ft_strncmp(tmp, http, this->ft_strlen(http)) != 0)
		throw BadRequest();
	str = tmp;
}

std::vector<std::string> Request::splitStartLine(std::string &str)
{
	std::vector<std::string> res;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	while (str[end] != '\0' && str[end] != ' ')
	{
		if ((str[end] < 'A' || str[end] > 'Z') && str[end] != '_' && str[end] != '-')
			throw BadRequest();
		++end;
	}
	if (end == 0)
		throw BadRequest();
	res.push_back(str.substr(start, end - start));
	this->checkMethod(res[0]);
	std::cout << "res[0]: " << res[0] << std::endl;
	end = skipChar(str, end, ' ');
	start = end;
	while (str[end] != '\0')
	{

		if (end != start && str[end] == 'H' && str[end - 1] == ' ')
			break;
		++end;
	}
	if (str[end] == '\0')
		throw BadRequest();
	res.push_back(str.substr(start, end - start));
	this->trimTail(res[1], ' ');
	this->checkTargetUri(res[1]);
	std::cout << "res[1]: " << res[1] << std::endl;
	start = end;
	while (str[end] != '\0')
	{
		if (str[end] == ' ')
			break;
		++end;
	}
	res.push_back(str.substr(start, end - start));
	std::cout << "res[2]: " << res[2] << std::endl;
	this->checkHttpVer(res[2]);
	if (str[end] == ' ')
	{
		end = skipChar(str, end, ' ');
		if (str[end] != '\0' && str[end] != ' ')
			throw BadRequest();
	}
	return (res);
}

std::string *Request::ft_split(std::string &str, char delim)
{
	int size = 0;
	std::string *res;

	std::string::size_type start = 0;
	this->trimTail(str, delim);
	start = skipChar(str, start, delim);
	std::string::size_type tmp_idx = start;
	while (str[start] != '\0')
	{
		if (str[start] == delim)
		{
			++size;
			start = skipChar(str, start, delim);
		}
		else
			++start;
	}
	if (size == 0)
		return (NULL);
	res = new std::string[size + 2];

	int i = 0;
	start = 0;
	std::string::size_type end = str.find(delim, tmp_idx);
	while (end != std::string::npos && i < size)
	{
		if (str[end] == delim)
		{
			res[i++] = str.substr(start, end - start);
			start = end;
			start = skipChar(str, start, delim);
			end = str.find(delim, start);
		}
	}
	res[i++] = str.substr(start);
	res[i] = "";
	return (res);
}

void Request::printLine(void)
{
	std::cout << "Result from parsing" << std::endl;
	std::cout << "Method: " << this->_start_line[0] << std::endl;
	std::cout << "Request target: " << this->_start_line[1] << std::endl;
	std::cout << "HTTP version: " << this->_start_line[2] << std::endl;
	std::cout << "Body: " << this->_body << std::endl;
	std::cout << "buffer size " << this->_buff_size << std::endl;
}

void Request::parseStartLine(void)
{
	std::vector<std::string> buffer = splitStartLine(this->_line[0]);
	int count = 0;
	for (std::string::size_type idx = 0; idx < 3; ++idx)
	{
		++count;
		this->_start_line[idx] = buffer[idx];
		std::cout << "startline: " << this->_start_line[idx] << std::endl;
	}
	std::cout << "count is " << count << std::endl;
	if (count < 3)
		throw BadRequest();
}

void Request::parseHeader(void)
{
	for (int i = 1; i < this->_buff_size; ++i)
	{
		std::cout << "test buff: " << this->_line[i] << std::endl;
		std::string *tmp = this->ft_split(this->_line[i], ':');
		if (tmp == NULL || tmp[0] == "" || tmp[0][0] == ' ' || tmp[0][0] == '\t' || tmp[0].back() == ' ' || tmp[0].back() == '\t' || (tmp[1][0] != ' ' && tmp[1][0] != '\t'))
			throw BadRequest();
		if (std::isalpha(tmp[0][0]) && (tmp[0][0] < 'A' || tmp[0][0] > 'Z'))
		{
			// std::cout << "yyy" << std::endl;
			tmp[0][0] -= 32;
		}

		for (int i = 1; tmp[0][i] != '\0'; ++i)
		{
			if (std::isalpha(tmp[0][i]) && (tmp[0][i] < 'a' || tmp[0][i] > 'z'))
			{
				// std::cout << "xxx" << std::endl;
				tmp[0][i] += 32;
			}
		}
		std::cout << "tmp[0]: " << tmp[0] << std::endl;
		delete[] tmp;
		tmp = NULL;
	}
}

void Request::parseRequest(void)
{
	try
	{
		this->_line = this->splitLine();
		for (int i = 0; i < this->_buff_size; ++i)
			this->trimTail(this->_line[i], '\r');
		this->parseStartLine();
		this->parseHeader();
		// this->printLine();
	}
	catch (const BadRequest &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (const HttpNotSupport &e)
	{
		std::cout << e.what() << std::endl;
	}
}
