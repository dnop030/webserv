#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	this->_config_ser = -1;
	this->_status[100] = "Continue";
	this->_status[101] = "Switching Protocols";
	this->_status[200] = "OK";
	this->_status[201] = "Created";
	this->_status[202] = "Accepted";
	this->_status[400] = "Bad Request";
	this->_status[404] = "Not Found";
	this->_status[405] = "Method Not Allowed";
	this->_status[411] = "Length Required";
	this->_status[413] = "Request Entity Too Large";
	this->_status[500] = "Internal Server Error";
	this->_status[501] = "Not Implemented";
	this->_status[505] = "HTTP Version not supported";
	this->_fileError[404] = "page/404.html";
	this->_fileError[405] = "page/405.html";
	this->_fileError[500] = "page/500.html";
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

void	HttpResponse::setServername(std::string const &servername)
{
	this->_serverName = servername;
}

void	HttpResponse::setPath(std::string const &path)
{
	this->_path = path;
}

void	HttpResponse::setPort(std::string const &port)
{
	this->_port = port;
}

void	HttpResponse::setMethod(std::string const &method)
{
	this->_method = method;
}

std::string	HttpResponse::_checkFile()
{
	std::ifstream ifs(this->_fileResponse);

	if (ifs.good()) {
		std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		return content;
	} else {
		std::string content("");
		return content;
	}
}

void	HttpResponse::setConfig(ConfigFileHandle *config)
{
	this->_config = config;
}

void	HttpResponse::_setHeader(std::string const &key, std::string const &value)
{
	this->_header[key] = value;
}

int	HttpResponse::_checkPort()
{
	unsigned int amount_ser = this->_config->getAmountServConfig();
	for (int i = 0; i < amount_ser; i++) {
		if (this->_port.compare(this->_config->getServConfigVal(i, "listen")) == 0) {
			this->_config_ser = i;
			break;
		}
	}
	return (this->_config_ser);
}

int	HttpResponse::_checkPath()
{
	this->_config_location = (this->_config_ser > -1) ? this->_config->getServConfigVal(this->_config_ser, "location " + this->_path) : "";

	return this->_config_location.length();
}

void	HttpResponse::_setConfigCondition()
{
	std::string condition = this->_config_location;
	std::string delimiter = ";";
	size_t pos = 0;
	std::string token;

	while ((pos = condition.find(delimiter)) != std::string::npos) {
		token = condition.substr(0, pos);
		this->_config_condition.push_back(token);
		condition.erase(0, pos + delimiter.length());
	}
	this->_config_condition.push_back(condition);
}

void	HttpResponse::_checkMethod()
{
	size_t 						pos = 0;
	std::string 				method = "allow_method", delimiter = " ", token;
	std::vector<std::string>	allow_method;

	for(auto value : this->_config_condition) {
		if (method == value.substr(0, method.length())) {
			method = value;
		}
	}
	if (method != "allow_method") {
		while ((pos = method.find(delimiter)) != std::string::npos) {
			token = method.substr(0, pos);
			allow_method.push_back(token);
			method.erase(0, pos + delimiter.length());
		}
		allow_method.push_back(method);

		for(auto value : allow_method) {
			if (value == this->_method)
				return ;
		}
		throw (405);
	}
}

std::string	HttpResponse::_setResponseStream()
{
	std::ostringstream resStream;
	std::string contentRes = this->_checkFile();
	std::map<std::string, std::string>::iterator it;

	this->_setHeader("Content-Length:", std::to_string(contentRes.length()));
	this->_setHeader("Content-Type:", "text/html");
	this->_setHeader("Accept-Language:", "TH");

	resStream << "HTTP/1.1 " << this->_statusCode << " " << this->_status[this->_statusCode] << "\r\n";
	for (it = this->_header.begin(); it != this->_header.end(); it++) {
		resStream << it->first << " " << it->second << "\r\n";
	}
	// resStream << "test-path: " << this->_path << "\r\n";
	// resStream << "test-condition: " << this->_config_location << "\r\n";
	resStream << "\r\n";
	resStream << contentRes;

	return resStream.str();
}

std::string	HttpResponse::returnResponse()
{
	try {
		if (this->_checkPort() > -1 && this->_checkPath()) {
			this->_setConfigCondition();
			this->_checkMethod();
			this->_statusCode = 200;
			this->_fileResponse = "page/index.html";
		} else
			throw (404);
	} catch (int status) {
		this->_statusCode = status;
		this->_fileResponse = this->_fileError[status];
	}

	return (this->_setResponseStream());
}