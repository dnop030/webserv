#include "HttpHandle.hpp"

HttpHandle::HttpHandle(std::string &buffer) : _statusCode(0), _buffer(buffer)
{
	this->_req = new Request(this->_buffer);
	// this->_req = Request(this->_buffer);
	this->_method = this->_req->getMethod();
	this->_hostname = this->_req->getHostname();
	this->_port = this->_req->getPort();
	this->_path = this->_req->getPath();
	this->_fragment = this->_req->getFragment();
	this->_query = this->_req->getQuery();
	this->_header = this->_req->getHeader();
	this->_body = this->_req->getBody();
	this->_bodyChunk = this->_req->getBodyChunk();

	this->printLine();
}

HttpHandle::~HttpHandle()
{
	if (this->_req != NULL)
	{
		delete this->_req;
		this->_req = NULL;
	}
}

HttpHandle::HttpHandle(HttpHandle const &src) : _buffer(src._buffer), _statusCode(src._statusCode), _req(src._req)
{
	*this = src;
}

HttpHandle &HttpHandle::operator=(HttpHandle const &src)
{
	if (this != &src)
	{
		// if (this->_req != NULL)
		// 	delete this->_req;
		std::string tmp_buff = src._buffer;
		// this->_req = new Request(tmp_buff);
		this->_method = src._method;
		this->_hostname = src._hostname;
		this->_port = src._port;
		this->_path = src._path;
		this->_fragment = src._fragment;
		if (!this->_query.empty())
			this->_query.clear();
		this->_query = const_cast<HttpHandle &>(src)._req->getQuery();
	}
	return (*this);
}

void HttpHandle::printLine(void)
{
	std::cout << "Result from parsing in HttpHandle" << std::endl;
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "<<<<<<<<< Header as below >>>>>>>>>>" << std::endl;
	this->_req->printMap(this->_header);
	std::cout << "<<<<<<<<< Body as below >>>>>>>>>>\n"
			  << this->_body << std::endl;
	std::cout << "\n<<<<<<<<< Body in chunk as below >>>>>>>>>>" << std::endl;
	for (std::vector<std::string>::iterator it = this->_bodyChunk.begin(); it != this->_bodyChunk.end(); ++it)
		std::cout << *it << "\n\nnewline\n"
				  << std::endl;
	std::cout << "hostname: " << this->_hostname << std::endl;
	std::cout << "port: " << this->_port << std::endl;
	std::cout << "path: " << this->_path << std::endl;
	std::cout << "fragment: " << this->_fragment << std::endl;
	std::cout << "query: ";
	this->_req->printMap(this->_query);
	std::cout << "\nstatus code is " << this->_statusCode << std::endl;
	std::cout << std::endl;
}
