#include "Request.hpp"

Request::Request(std::string &buffer) : _buffer(buffer), _line(0), _buff_size(0), _method(""), _port(""), _hostname(""), _path(""), _fragment("")
{
}

Request::Request(Request const &src) : _buff_size(src._buff_size), _buffer(src._buffer), _method(src._method), _port(src._port), _hostname(src._hostname), _path(src._path), _fragment(src._fragment)
{
	*this = src;
}

Request &Request::operator=(Request const &src)
{
	if (this != &src)
	{
		for (int i = 0; i < src._buff_size; ++i)
			this->_line[i] = src._line[i];
		if (this->_query.empty())
			this->_query.clear();
		std::map<std::string, std::string>::const_iterator it2 = src._query.begin();
		while (it2 != src._query.end())
		{
			this->_query[it2->first] = it2->second;
			++it2;
		}
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

bool Request::endLine(std::string &buffer, std::string::size_type idx)
{
	if (buffer[idx] == '\n' && buffer[idx + 1] != '\0')
	{
		if (buffer[idx + 1] == '\n')
			return (true);
		if (buffer[idx + 1] == '\r' && buffer[idx + 2] != '\0' && buffer[idx + 2] == '\n')
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

	std::string::size_type start = 0;
	std::string::size_type end = this->_buffer.find('\n');
	while (end != std::string::npos && !this->endLine(this->_buffer, end))
	{
		if (this->_buffer[end] == '\n')
		{
			if (start != end)
				res.push_back(this->_buffer.substr(start, end - start));
			start = end + 1;
			end = this->_buffer.find('\n', start);
		}
	}
	if (this->_buffer[end] == '\n' && start != end)
	{
		res.push_back(this->_buffer.substr(start, end - start));
		start = end + 1;
	}
	this->_buff_size = 0;
	for (std::vector<std::string>::iterator it = res.begin(); it != res.end(); ++it)
		this->_buff_size += 1;
	if (this->_buff_size <= 2)
	{
		throw BadRequest();
	}
	while (this->_buffer[start] != '\0' && (this->_buffer[start] == '\n' || this->_buffer[start] == '\r'))
		++start;
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

double Request::ft_stod(const std::string &s)
{
	double res = 0;

	res = atof(s.c_str());
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

std::string::size_type Request::getLastChar(std::string &str)
{
	std::string::size_type res = 0;
	while (str[res] != '\0')
		++res;
	return (res);
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

bool Request::isIPv4(std::string &str)
{
	std::cout << "In Ipv4" << std::endl;
	std::string *chunk = NULL;
	std::string::size_type end = str.find_first_of(':');
	if (end == std::string::npos)
		end = str.find_first_of('/');
	if (end == std::string::npos)
		chunk = this->ft_split(str, '.');
	else
	{
		std::string str_tmp = str.substr(0, end - 0);
		chunk = this->ft_split(str_tmp, '.');
	}
	if (chunk == NULL)
		return (false);
	int i = 0;
	while (!chunk[i].empty())
	{
		if (chunk[i] == "" || !this->allDigit(chunk[i]))
		{
			return (false);
			delete[] chunk;
		}
		double tmp = this->ft_stod(chunk[i]);
		if (tmp < 0 || tmp > 255)
		{
			return (false);
			delete[] chunk;
		}
		++i;
	}
	delete[] chunk;
	std::cout << "i is " << i << std::endl;
	if (i != 4)
		return (false);
	std::cout << "ipv4 => " << str << std::endl;
	return (true);
}

void Request::parsePort(std::string &dns)
{
	std::string::size_type start = dns.find_first_of(':');
	std::string::size_type end = dns.find_first_of('/');
	if (start != std::string::npos)
	{
		this->_hostname = dns.substr(0, start);
		if (end != std::string::npos)
			this->_port = dns.substr(++start, --end - start);
		else
		{
			end = this->getLastChar(dns);
			this->_port = dns.substr(++start, end - start);
		}
	}
	if (this->_port.empty() || !this->allDigit(this->_port))
		throw BadRequest();
}

void Request::parsePath(std::string &dns)
{
	std::string::size_type start = dns.find_first_of('/');
	std::string::size_type end = dns.find_first_of('?');
	if (start != std::string::npos)
	{
		if (end == std::string::npos)
			end = dns.find_first_of('#');
		if (end != std::string::npos)
			this->_path = dns.substr(start, end - start);
		else
		{
			end = this->getLastChar(dns);
			this->_path = dns.substr(start, end - start);
		}
	}
}

void Request::parseQuery(std::string &dns)
{
	std::string tmp = "";
	std::string::size_type start = dns.find_first_of('?');
	std::string::size_type end = dns.find_first_of('#');
	if (start != std::string::npos)
	{
		if (end != std::string::npos)
			tmp = dns.substr(++start, --end - start);
		else
		{
			end = this->getLastChar(dns);
			tmp = dns.substr(++start, end - start);
		}
		std::string *query = this->ft_split(tmp, '&');
		std::string *map = NULL;
		if (query == NULL)
		{
			map = this->ft_split(tmp, '=');
			if (this->_query.find(map[0]) == this->_query.end())
				this->_query[map[0]] = map[1];
			delete[] map;
			map = NULL;
		}
		for (int i = 0; !query[i].empty(); ++i)
		{
			map = this->ft_split(query[i], '=');
			if (this->_query.find(map[0]) == this->_query.end())
				this->_query[map[0]] = map[1];
			delete[] map;
			map = NULL;
		}
		if (map != NULL)
		{
			delete[] map;
			map = NULL;
		}
		this->printMap(this->_query);
	}
}

void Request::parseFragment(std::string &dns)
{
	std::string::size_type start = dns.find_first_of('#');
	if (start == std::string::npos)
		return;
	std::string::size_type end = this->getLastChar(dns);
	if (++start == end)
		this->_fragment = "";
	else
		this->_fragment = dns.substr(start, end - start);
}

void Request::checkDNS(std::string &dns)
{
	std::string::size_type start = 0;
	std::string::size_type end = this->getLastChar(dns) - 1;

	if (!std::isalpha(dns[0]) || (!std::isalpha(dns[end]) && !std::isdigit(dns[end])))
		throw BadRequest();
	for (int i = 0; dns[i] != '\0'; ++i)
	{
		if (dns[i] == '.' && dns[i + 1] == '.')
			throw BadRequest();
	}
	std::string *tmp = this->ft_split(dns, '.');
	std::vector<std::string> chunk;
	if (tmp == NULL)
		chunk.push_back(dns);
	else
	{
		for (int i = 0; !tmp[i].empty(); ++i)
			chunk.push_back(tmp[i]);
	}
	delete[] tmp;
	for (std::vector<std::string>::iterator it = chunk.begin(); it != chunk.end(); ++it)
	{
		std::string str = *it;
		end = this->getLastChar(str) - 1;
		if (!std::isalpha(str[0]) && !std::isdigit(str[0]) && !std::isalpha(str[end]) && !std::isdigit(str[end]))
			throw BadRequest();
		for (std::string::size_type i = 1; i < end; ++i)
		{
			if (!std::isalpha(str[i]) && !std::isdigit(str[i]) && str[i] != '-')
				throw BadRequest();
		}
	}
}

void Request::checkTargetUri(std::string &str)
{
	// str = "http://www.mastermind42.com:9090/path?query=ferret&query2=ferret2&query3=ferret3#nose";
	//     str = "http://255.255.255.255:9090/path?query=ferret&query2=ferret2";
	if (str[0] == '/')
	{
		if (this->_header.find("Host") == this->_header.end())
			throw BadRequest();
		this->parsePort(this->_header["Host"]);
		// this->parsePath(this->_header["Host"]);
		this->_path = str[0];
		this->parseQuery(this->_header["Host"]);
		this->parseFragment(this->_header["Host"]);
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
	std::string dns = str.substr(this->ft_strlen(http));
	this->parsePort(dns);
	this->parsePath(dns);
	this->parseQuery(dns);
	this->parseFragment(dns);
	if (isIPv4(this->_hostname))
		return;
	this->checkDNS(this->_hostname);
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
	start = end;
	while (str[end] != '\0')
	{
		if (str[end] == ' ')
			break;
		++end;
	}
	res.push_back(str.substr(start, end - start));
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
	std::cout << "<<<<<<<<< Header as below >>>>>>>>>>" << std::endl;
	this->printMap(this->_header);
	std::cout << "<<<<<<<<< Body as below >>>>>>>>>>\n"
			  << this->_body << std::endl;
	std::cout << "buffer size " << this->_buff_size << std::endl;
	std::cout << "hostname: " << this->_hostname << std::endl;
	std::cout << "port: " << this->_port << std::endl;
	std::cout << "path: " << this->_path << std::endl;
	std::cout << "fragment: " << this->_fragment << std::endl;
	std::cout << "query: ";
	this->printMap(this->_query);
	std::cout << std::endl;
}

void Request::printMap(std::map<std::string, std::string> &map)
{
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

void Request::parseStartLine(void)
{
	std::vector<std::string> buffer = splitStartLine(this->_line[0]);
	int count = 0;
	for (std::string::size_type idx = 0; idx < 3; ++idx)
	{
		++count;
		this->_start_line[idx] = buffer[idx];
	}
	if (count < 3)
		throw BadRequest();
}

void Request::parseHeader(void)
{
	for (int i = 1; i < this->_buff_size; ++i)
	{
		std::string tmp[2];
		size_t end = this->_line[i].find_first_of(':');
		if (end == std::string::npos)
			throw BadRequest();
		tmp[0] = this->_line[i].substr(0, end);
		if (++end == std::string::npos)
			tmp[1] = "";
		else
			tmp[1] = this->_line[i].substr(end);
		if (tmp == NULL || tmp[0] == "" || tmp[0][0] == ' ' || tmp[0][0] == '\t' || tmp[0].back() == ' ' || tmp[0].back() == '\t' || (tmp[1][0] != ' ' && tmp[1][0] != '\t'))
			throw BadRequest();
		if (std::isalpha(tmp[0][0]) && (tmp[0][0] < 'A' || tmp[0][0] > 'Z'))
			tmp[0][0] -= 32;

		for (int i = 1; tmp[0][i] != '\0'; ++i)
		{
			if (std::isalpha(tmp[0][i]) && (tmp[0][i] < 'a' || tmp[0][i] > 'z'))
				tmp[0][i] += 32;
		}
		if (this->_header.find(tmp[0]) != this->_header.end())
			throw BadRequest();
		std::string::size_type start = 0;
		start = this->skipChar(tmp[1], start, ' ');
		start = this->skipChar(tmp[1], start, '\t');
		start = this->skipChar(tmp[1], start, ' ');
		if (tmp[1][start] == '\0')
			this->_header[tmp[0]] = "";
		else
			this->_header[tmp[0]] = tmp[1].substr(start);
	}
}

void Request::parseRequest(void)
{
	try
	{
		this->_line = this->splitLine();
		for (int i = 0; i < this->_buff_size; ++i)
			this->trimTail(this->_line[i], '\r');
		this->parseHeader();
		this->parseStartLine();
		this->printLine();
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
