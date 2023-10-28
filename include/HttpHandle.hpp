#pragma once

#include "Http.hpp"
#include "Request.hpp"

// class Http;
//  class Request;
class HttpHandle
{
private:
	Request *_req;
	// Http *_req;
	// Response *_res;
	int _statusCode;
	double _contentLength;
	std::string _buffer;
	std::string _method;
	std::string _hostname;
	std::string _port;
	std::string _path;
	std::string _fragment;
	std::string _body;
	std::map<std::string, std::string> _query;
	std::map<std::string, std::string> _header;
	std::vector<std::string> _bodyChunk;

public:
	HttpHandle(std::string &buffer);
	~HttpHandle();
	HttpHandle(HttpHandle const &src);
	HttpHandle &operator=(HttpHandle const &src);

	void printLine(void);
};
