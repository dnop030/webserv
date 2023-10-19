#pragma once

#include "Http.hpp"
#include "Request.hpp"

class HttpHandle
{
private:
	Request *_req;
	// Response *_res;
	int _statusCode;
	std::string _buffer;
	std::string _method;
	std::string _hostname;
	std::string _port;
	std::string _path;
	std::string _fragment;
	std::map<std::string, std::string> _query;

public:
	HttpHandle(std::string &buffer);
	~HttpHandle();
	HttpHandle(HttpHandle const &src);
	HttpHandle &operator=(HttpHandle const &src);
};
