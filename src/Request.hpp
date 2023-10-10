#pragma once

#include "Http.hpp"
#include <vector>
#include <cctype>
#include <sstream>

class Request : Http
{
private:
	int _buff_size;
	std::string _buffer;
	std::vector<std::string> _line;
	std::string _method;

public:
	Request(std::string &buffer);
	Request(Request const &src);
	Request &operator=(Request const &src);

	bool endLine(std::string::size_type idx);
	int countChar(std::string &str, char c);
	void removeChar(std::string &str, char c);
	int countReturn(std::string &str);
	void trimTail(std::string &str, char delim);

	std::vector<std::string> splitLine(void);
	std::string::size_type skipChar(std::string &str, std::string::size_type idx, char c);
	int ft_stoi(const std::string &s);
	size_t ft_strlen(std::string &str);
	int allDigit(std::string &str);
	int ft_strncmp(std::string &str1, std::string &str2, size_t size);
	void checkMethod(std::string &str);
	void checkTargetUri(std::string &str);
	void checkHttpVer(std::string &str);
	std::vector<std::string> splitStartLine(std::string &str);
	std::string *ft_split(std::string &str, char delim);

	void parseStartLine(void);
	void parseHeader(void);
	void parseRequest(void);
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
	void printLine(void);
};
