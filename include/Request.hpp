#pragma once

#include "Http.hpp"
#include <vector>
#include <cctype>
#include <sstream>
#include "color.hpp"
#include "FormData.hpp"
#include "HttpHandle.hpp"
#include "CheckHeader.hpp"
#include "UtilsFunction.hpp"

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
	int countReturn(std::string &str);
	std::string::size_type findCLRF(std::string &str, std::string::size_type idx);

	std::vector<std::string> splitLine(void);
	std::vector<std::string> splitStartLine(std::string &str);

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

	void getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound);

	friend class FormData;
	friend class CheckHeader;
	friend class HttpHandle;
};
