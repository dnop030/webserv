#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
}

HttpResponse::~HttpResponse()
{
}

HttpResponse::HttpResponse(HttpResponse const &rhs)
{
	*this = rhs;
}

HttpResponse	&HttpResponse::operator=(HttpResponse const &rhs)
{
	if (this == &rhs)
		return *this;
	// do something
	return *this;
}

void	HttpResponse::setStatusCode(int statusCode)
{
	this->_statusCode = statusCode;
}

void	HttpResponse::setStatusMessage(std::string const &statusMessage)
{
	this->_statusMessage = statusMessage;
}

void	HttpResponse::setFileResponse(std::string const &fileResponse)
{
	this->_fileResponse = fileResponse;
}

std::string	HttpResponse::checkFile()
{
	std::ifstream ifs(this->_fileResponse);

	if (ifs.good()) {
		std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		return content;
	} else {
		this->_statusCode = 404;
		this->_statusMessage = "Not Found";
		std::string content("<h1>404 not found</h1>");
		return content;
	}
}

void	HttpResponse::setHeader(std::string const &key, std::string const &value)
{
	this->_header[key] = value;
}

std::string	HttpResponse::returnResponse()
{
	std::ostringstream resStream;
	std::ifstream ifs(this->_fileResponse);
	std::string contentRes = this->checkFile();
	std::map<std::string, std::string>::iterator it;

	setHeader("Content-Length:", std::to_string(contentRes.length()));
	setHeader("Content-Type:", "text/html");
	setHeader("Accept-Language:", "TH");

	resStream << "HTTP/1.1 " << this->_statusCode << " " << this->_statusMessage << "\r\n";
	for (it = this->_header.begin(); it != this->_header.end(); it++) {
		resStream << it->first << " " << it->second << "\r\n";
	}
	resStream << "\r\n";
	resStream << contentRes;

	return resStream.str();
}