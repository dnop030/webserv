#include "Request.hpp"

Request::Request(std::string &buffer) : _buffer(buffer), _line(0), _buff_size(0), _method(""), _port(""), _hostname(""), _path(""), _fragment(""), _bodySize(0), _statusCode(0)
{
	try
	{
		this->_line = this->splitLine();
		for (int i = 0; i < this->_buff_size; ++i)
			trimTail(this->_line[i], '\r');
		this->parseHeader();
		this->parseStartLine();
		this->parseBody();
	}
	catch (const BadRequest &e)
	{
		this->_statusCode = 400;
		std::cout << e.what() << std::endl;
	}
	catch (const HttpNotSupport &e)
	{
		this->_statusCode = 505;
		std::cout << e.what() << std::endl;
	}
	catch (const LengthRequired &e)
	{
		this->_statusCode = 411;
		std::cout << e.what() << std::endl;
	}
	catch (const NotImplement &e)
	{
		this->_statusCode = 501;
		std::cout << e.what() << std::endl;
	}
}

Request::Request(Request const &src) : Http(src), _buff_size(src._buff_size), _buffer(src._buffer), _method(src._method), _port(src._port), _hostname(src._hostname), _path(src._path), _fragment(src._fragment), _bodySize(src._bodySize), _statusCode(src._statusCode)
{
	*this = src;
}

Request &Request::operator=(Request const &src)
{
	if (this != &src)
	{
		for (std::vector<std::string>::const_iterator it = src._line.begin(); it != src._line.end(); ++it)
			this->_line.push_back(*it);
		if (!this->_query.empty())
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

const char *Request::LengthRequired::what() const throw()
{
	return ("411 Length required");
}

const char *Request::NotImplement::what() const throw()
{
	return ("501 not implemented");
}

bool Request::endLine(std::string &buffer, std::string::size_type idx)
{
	if (buffer[idx] == '\r' && buffer[idx + 1] != '\0' && buffer[idx + 1] == '\n')
	{
		++idx;
		if (buffer[idx + 1] == '\r' && buffer[idx + 2] != '\0' && buffer[idx + 2] == '\n')
			return (true);
	}
	return (false);
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

std::string::size_type Request::findCLRF(std::string &str, std::string::size_type idx)
{
	while (str[idx] != '\0')
	{
		if (str[idx] == '\r' && str[idx + 1] != '\0' && str[idx + 1] == '\n')
			break;
		++idx;
	}
	if (str[idx] == '\r' && str[idx + 1] != '\0' && str[idx + 1] == '\n')
		return (idx);
	return (std::string::npos);
}

std::vector<std::string> Request::splitLine(void)
{
	std::vector<std::string> res;

	std::string::size_type start = 0;
	std::string::size_type end = this->findCLRF(this->_buffer, start);
	while (end != std::string::npos && !this->endLine(this->_buffer, end))
	{
		if (this->_buffer[end] == '\r' && this->_buffer[end + 1] == '\n')
		{
			if (start != end)
				res.push_back(this->_buffer.substr(start, end - start));
			end += 2;
			start = end;
			end = this->findCLRF(this->_buffer, start);
		}
	}
	if (this->_buffer[end] == '\r' && this->_buffer[end + 1] == '\n' && start != end)
	{
		res.push_back(this->_buffer.substr(start, end - start));
		start = end + 2;
	}
	this->_buff_size = 0;
	for (std::vector<std::string>::iterator it = res.begin(); it != res.end(); ++it)
		this->_buff_size += 1;
	if (this->_buff_size <= 2)
		throw BadRequest();
	while (this->_buffer[start] != '\0' && (this->_buffer[start] == '\n' || this->_buffer[start] == '\r'))
		++start;
	if (this->_buffer[start] != '\0')
	{
		this->_body = this->_buffer.substr(start);
		this->_bodySize = this->_body.size();
	}
	return (res);
}

void Request::checkHttpVer(std::string &str)
{
	std::string tmp = "HTTP";
	std::string *buffslash = ft_split(str, '/');
	if (buffslash == NULL || ft_strncmp(buffslash[0], tmp, ft_strlen(tmp) != 0))
	{
		delete[] buffslash;
		throw BadRequest();
	}
	std::string *buffdot = ft_split(buffslash[1], '.');
	delete[] buffslash;
	buffslash = NULL;
	if (buffdot == NULL)
		throw BadRequest();
	int size = 0;
	while (!buffdot[size].empty())
		++size;
	if (size != 2 || !allDigit(buffdot[0]) || !std::isdigit(buffdot[1][0]))
	{
		delete[] buffdot;
		throw BadRequest();
	}
	tmp = "HTTP/1.1";
	if (ft_strncmp(str, tmp, ft_strlen(tmp)) != 0)
	{
		delete[] buffdot;
		throw HttpNotSupport();
	}
	if (!allDigit(buffdot[1]))
	{
		delete[] buffdot;
		throw BadRequest();
	}
	if (buffdot != NULL)
	{
		delete[] buffdot;
		buffdot = NULL;
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
		throw NotImplement();
}

bool Request::isIPv4(std::string &str)
{
	std::cout << "In Ipv4" << std::endl;
	std::string *chunk = NULL;
	std::string::size_type end = str.find_first_of(':');
	if (end == std::string::npos)
		end = str.find_first_of('/');
	if (end == std::string::npos)
		chunk = ft_split(str, '.');
	else
	{
		std::string str_tmp = str.substr(0, end - 0);
		chunk = ft_split(str_tmp, '.');
	}
	if (chunk == NULL)
		return (false);
	int i = 0;
	while (!chunk[i].empty())
	{
		if (chunk[i] == "" || !allDigit(chunk[i]))
		{
			return (false);
			delete[] chunk;
		}
		double tmp = ft_stod(chunk[i]);
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
			end = getLastChar(dns);
			this->_port = dns.substr(++start, end - start);
		}
	}
	if (this->_port.empty() || !allDigit(this->_port))
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
			end = getLastChar(dns);
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
			end = getLastChar(dns);
			tmp = dns.substr(++start, end - start);
		}
		std::string *query = ft_split(tmp, '&');
		std::string *map = NULL;
		if (query == NULL)
		{
			map = ft_split(tmp, '=');
			if (this->_query.find(map[0]) == this->_query.end())
				this->_query[map[0]] = map[1];
			delete[] map;
			map = NULL;
		}
		for (int i = 0; !query[i].empty(); ++i)
		{
			map = ft_split(query[i], '=');
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
		if (query != NULL)
		{
			delete[] query;
			query = NULL;
		}
		this->printMap(this->_query);
	}
}

void Request::parseFragment(std::string &dns)
{
	std::string::size_type start = dns.find_first_of('#');
	if (start == std::string::npos)
		return;
	std::string::size_type end = getLastChar(dns);
	if (++start == end)
		this->_fragment = "";
	else
		this->_fragment = dns.substr(start, end - start);
}

void Request::checkDNS(std::string &dns)
{
	std::string::size_type start = 0;
	std::string::size_type end = getLastChar(dns) - 1;

	if (!std::isalpha(dns[0]) || (!std::isalpha(dns[end]) && !std::isdigit(dns[end])))
		throw BadRequest();
	for (int i = 0; dns[i] != '\0'; ++i)
	{
		if (dns[i] == '.' && dns[i + 1] == '.')
			throw BadRequest();
	}
	std::string *tmp = ft_split(dns, '.');
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
		end = getLastChar(str) - 1;
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
	if (str[0] == '/')
	{
		if (this->_header.find("Host") == this->_header.end())
			throw BadRequest();
		this->parsePort(this->_header["Host"]);
		// this->_path = str[0];
		this->_path = str;
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
	if (ft_strncmp(tmp, http, ft_strlen(http)) != 0)
		throw BadRequest();
	str = tmp;
	std::string dns = str.substr(ft_strlen(http));
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
	trimTail(res[1], ' ');
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
	std::cout << "<<<<<<<<< Body in chunk as below >>>>>>>>>>" << std::endl;
	for (std::vector<std::string>::iterator it = this->_bodyChunk.begin(); it != this->_bodyChunk.end(); ++it)
		std::cout << *it << "\n\nnewline\n"
				  << std::endl;
	std::cout << "Body size: " << this->_bodySize << std::endl;
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
	{
		if (it->first == "body")
		{
			std::cout << "body: as below" << std::endl;
			std::cout << it->second << std::endl;
		}
		else
			std::cout << it->first << ": " << it->second << std::endl;
	}
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
		std::string::size_type find = tmp[0].find_first_of('-');
		if (find != std::string::npos)
		{
			find += 1;
			if (std::isalpha(tmp[0][find]) && (tmp[0][find] < 'A' || tmp[0][find] > 'Z'))
				tmp[0][find] -= 32;
		}
		if (this->_header.find(tmp[0]) != this->_header.end())
			throw BadRequest();
		std::string::size_type start = 0;
		start = skipChar(tmp[1], start, ' ');
		start = skipChar(tmp[1], start, '\t');
		start = skipChar(tmp[1], start, ' ');
		if (tmp[1][start] == '\0')
			this->_header[tmp[0]] = "";
		else
			this->_header[tmp[0]] = tmp[1].substr(start);
	}
	CheckHeader header(*this);
	deepCopyMap(header._respHeader, this->_respHeader);
	if (this->_statusCode == 400)
		throw BadRequest();
	if (this->_statusCode == 411)
		throw LengthRequired();
}

void Request::parseBody(void)
{
	if (this->_method != "POST")
		return;
	FormData form(*this);
	if (this->_statusCode == 400)
		throw BadRequest();
	if (!this->_body.empty() && this->_header.find("Content-Length") != this->_header.end())
	{
		if (!allDigit(this->_header["Content-Length"]))
			throw BadRequest();
		double tmp = ft_stod(this->_header["Content-Length"]);
		std::string::size_type contentLen =
			static_cast<std::string::size_type>(tmp);
		if (contentLen != this->_bodySize)
			throw BadRequest();
		this->_body = this->_body.substr(0, contentLen);
	}
	else if (!this->_body.empty() && this->_header.find("Transfer-Encoding") != this->_header.end() && this->_header["Transfer-Encoding"] == "chunked")
	{
		std::string res;
		std::string tmp;
		std::string::size_type start = 0;
		std::string::size_type end = this->findCLRF(this->_body, start);
		int len_flag = 0;
		int len = 0;
		while (end != std::string::npos)
		{
			tmp = this->_body.substr(start, end - start);
			if (len_flag == 0 || len_flag % 2 == 0)
				len = ft_htod(tmp);
			if ((len_flag == 0 || len_flag % 2 == 0) && len < 0)
				throw BadRequest();
			else if (len_flag % 2 != 0 && len > 0)
			{
				if (len != tmp.size())
					throw BadRequest();
				this->_bodyChunk.push_back(tmp);
				res.append(tmp);
			}
			++len_flag;
			end += 2;
			start = end;
			end = this->findCLRF(this->_body, start);
		}
		if (len != 0 || (len == 0 && this->_body[start] != '\0'))
			throw BadRequest();
		this->_body = res;
	}
}

std::string Request::getMethod()
{
	return (this->_method);
}

std::string Request::getHostname()
{
	return (this->_hostname);
}

std::string Request::getPort()
{
	return (this->_port);
}

std::string Request::getPath()
{
	return (this->_path);
}

std::string Request::getFragment()
{
	return (this->_fragment);
}

std::map<std::string, std::string> Request::getQuery()
{
	std::map<std::string, std::string> res;
	if (this->_query.empty())
		return (res);
	for (std::map<std::string, std::string>::iterator it = this->_query.begin(); it != this->_query.end(); ++it)
	{
		if (res.find(it->first) == res.end())
			res[it->first] = it->second;
	}
	return (res);
}

int Request::getStatusCode()
{
	return (this->_statusCode);
}
