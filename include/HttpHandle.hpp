#pragma once

#include "Http.hpp"
#include "Request.hpp"
#include "HttpResponse.hpp"
#include "UtilsFunction.hpp"

class Request;
class HttpHandle
{
private:
	std::string _buffer;
	Request *_req;
	int _statusCode;
	double _contentLength;
	std::string _method;
	std::string _hostname;
	std::string _port;
	std::string _path;
	std::string _fragment;
	std::string _body;
	std::string _filename;
	std::map<std::string, std::string> _query;
	std::map<std::string, std::string> _header;
	std::vector<std::string> _bodyChunk;

public:
	HttpHandle(std::string &buffer);
	~HttpHandle();
	HttpHandle(HttpHandle const &src);
	HttpHandle &operator=(HttpHandle const &src);

	void addResponseHeader();

	void printLine(void);
	void printHeader(void);
	void printResponseHeader();

	std::string getConnection();
	HttpResponse response;
};
