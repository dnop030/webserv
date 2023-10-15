#include "HttpHandle.hpp"

HttpHandle::HttpHandle(std::string &buffer) : _statusCode(0), _req(NULL), _buffer("")
{
	this->_req = new Request(buffer);
}

HttpHandle::~HttpHandle()
{
	if (this->_req != NULL)
	{
		delete this->_req;
		this->_req = NULL;
	}
}

HttpHandle::HttpHandle(HttpHandle const &src) : _buffer(src._buffer), _statusCode(src._statusCode)
{
	*this = src;
}

HttpHandle &HttpHandle::operator=(HttpHandle const &src)
{
	if (this != &src)
	{
		if (this->_req != NULL)
			delete this->_req;
		std::string tmp_buff = src._buffer;
		this->_req = new Request(tmp_buff);
	}
	return (*this);
}
