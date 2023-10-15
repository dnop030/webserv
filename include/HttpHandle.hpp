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

public:
	HttpHandle(std::string &buffer);
	~HttpHandle();
	HttpHandle(HttpHandle const &src);
	HttpHandle &operator=(HttpHandle const &src);
};
