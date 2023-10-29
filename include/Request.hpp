#pragma once

#include "Http.hpp"
#include <vector>
#include <cctype>
#include <sstream>
#include "color.hpp"
#include "FormData.hpp"
#include "CheckHeader.hpp"

class Request : public Http
{
private:
	int _buff_size;
	int _statusCode;
	std::string::size_type _bodySize;
	std::string _buffer;
	std::vector<std::string> _line;
	std::string _method;
	std::string _hostname;
	std::string _port;
	std::string _path;
	std::string _fragment;
	std::map<std::string, std::string> _query;

public:
	Request(std::string &buffer);
	Request(Request const &src);
	Request &operator=(Request const &src);

	bool endLine(std::string &buffer, std::string::size_type idx);
	int countChar(std::string &str, char c);
	void removeChar(std::string &str, char c);
	int countReturn(std::string &str);
	void trimTail(std::string &str, char delim);
	void trimTail_str(std::string &str, std::string delim);
	std::string::size_type skipChar(std::string &str, std::string::size_type idx, char c);
	double ft_stod(const std::string &s);
	size_t ft_strlen(std::string &str);
	int allDigit(std::string &str);
	int ft_strncmp(std::string &str1, std::string &str2, size_t size);
	bool isHex(std::string &str);
	int ft_htod(std::string &str);
	std::string::size_type getLastChar(std::string &str);
	std::string::size_type findCLRF(std::string &str, std::string::size_type idx);

	std::vector<std::string> splitLine(void);
	std::vector<std::string> splitStartLine(std::string &str);
	std::string *ft_split(std::string &str, char delim);

	bool isIPv4(std::string &str);

	void checkMethod(std::string &str);
	void checkTargetUri(std::string &str);
	void checkHttpVer(std::string &str);
	void checkContentType();

	void parsePort(std::string &dns);
	void parsePath(std::string &dns);
	void parseQuery(std::string &dns);
	void parseFragment(std::string &dns);
	void parseStartLine(void);
	void parseHeader(void);
	void parseBody(void);
	void parseRequest(void);

	void checkDNS(std::string &dns);

	std::string getMethod();
	std::string getHostname();
	std::string getPort();
	std::string getPath();
	std::string getFragment();
	std::map<std::string, std::string> getQuery();
	int getStatusCode();
	~Request();

	class BadRequest : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class HttpNotSupport : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class LengthRequired : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class NotImplement : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	void printLine(void);
	void printMap(std::map<std::string, std::string> &map);

	std::string *ft_split_dummy(std::string &str, std::string delim);
	// std::string getFromBound(std::string::size_type &start, std::string &bound);
	void getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound);

	friend class FormData;
	friend class CheckHeader;
};
