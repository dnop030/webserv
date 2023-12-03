#include "HttpHandle.hpp"

HttpHandle::HttpHandle(std::string &buffer) : _statusCode(0), _buffer(buffer), _contentLength(0)
{
	this->_req = new Request(this->_buffer);
	this->_method = this->_req->getMethod();
	this->_hostname = this->_req->getHostname();
	this->_port = this->_req->getPort();
	this->_path = this->_req->getPath();
	this->_fragment = this->_req->getFragment();
	this->_query = this->_req->getQuery();
	this->_header = this->_req->getHeader();
	this->_body = this->_req->getBody();
	this->_bodyChunk = this->_req->getBodyChunk();
	if (this->_header.find("Content-Length") != this->_header.end())
		this->_contentLength = ft_stod(this->_header["Content-Length"]);
	// necessary to check config file [reponse]
	this->response.setServername(this->_hostname);
	this->response.setPath(this->_path);
	this->response.setPort(this->_port);
	this->response.setMethod(this->_method);
	this->response.setConnection(this->_header["Connection"]);
	this->response.setBody(this->_body);
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
		std::string tmp_buff = src._buffer;
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
	std::cout << "x<<<<<<<<< Body as below >>>>>>>>>>\n"
			  << this->_body << std::endl;
	std::cout << "<<<<<<<<< Body in chunk as below >>>>>>>>>>" << std::endl;
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
	std::cout << "\n@@@@@ Respond header @@@@@" << std::endl;
	this->printResponseHeader();
	std::cout << "@@@@@ End of Respond header @@@@@" << std::endl;
	std::cout << "Current date" << std::endl;
	std::cout << currentDate() << std::endl;
}

void HttpHandle::addResponseHeader(void)
{
	// Allow => lists of method that resources allowed => get from Petch
	// Location => indicates the URL to redirect a page to => get from Petch
	// Content-Type => type of resources to send back to client => get from Petch
	// Content-Length => size of resources to send back to client => get from Petch
	// Content-Location => indicates an alternate location(url) for the returned data => get from Petch
	// Last-Modified => contains a date and time when the origin server believes the resource was last modified => get from Petch
}

void HttpHandle::printResponseHeader(void)
{
	std::string tmp;
	std::string date = currentDate();
	addKey(this->_req->_respHeader, "Date", date);
	std::vector<std::string> vec = {"Date", "Allow", "Location", "Connection", "Content-Type", "Content-Length", "Accept-Language", "Server", "Content-Location", "Last-Modified"};
	for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); ++it)
		printKeyValue(this->_req->_respHeader, *it);
}

std::string HttpHandle::getConnection()
{
	if (this->_header.find("Connection") == this->_header.end())
		return ("keep-alive");
	else
		return (this->_header["Connection"]);
}
