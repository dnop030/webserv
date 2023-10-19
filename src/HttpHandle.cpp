#include "HttpHandle.hpp"

HttpHandle::HttpHandle(std::string &buffer) : _statusCode(0), _req(NULL), _buffer(buffer)
{
	this->_req = new Request(this->_buffer);
	this->_method = this->_req->getMethod();
	this->_hostname = this->_req->getHostname();
	this->_port = this->_req->getPort();
	this->_path = this->_req->getPath();
	this->_fragment = this->_req->getFragment();
	this->_query = this->_req->getQuery();
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
		this->_method = src._req->getMethod();
		this->_hostname = src._req->getHostname();
		this->_port = src._req->getPort();
		this->_path = src._req->getPath();
		this->_fragment = src._req->getFragment();
		if (!this->_query.empty())
			this->_query.clear();
		this->_query = src._req->getQuery();
	}
	return (*this);
}
