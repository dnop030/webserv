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

std::string	HttpResponse::returnResponse()
{
	std::ostringstream responseStream;
	std::ifstream ifs(this->_fileResponse);
	std::string contentResponse = this->checkFile();

	responseStream << "HTTP/1.1 " << this->_statusCode << " " << this->_statusMessage << "\r\n";
	responseStream << "Content-Length: " << contentResponse.length() << "\r\n";
	responseStream << "Content-Type: text/html\r\n";
	responseStream << "\r\n";
	responseStream << contentResponse;

	return responseStream.str();
}