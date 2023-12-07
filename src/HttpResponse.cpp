#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	this->_config_ser = -1;
	this->_statusCode = 200;
	this->_checkCGI = 0;
	this->_autoIndex = 0;
	this->_config_cgi_path = "";
	this->_url = "";
	this->_filenameDelete = "";
	this->_filename = "";
	this->_return = "";
	this->_status[100] = "Continue";
	this->_status[101] = "Switching Protocols";
	this->_status[200] = "OK";
	this->_status[201] = "Created";
	this->_status[202] = "Accepted";
	this->_status[301] = "Moved Permanently";
	this->_status[400] = "Bad Request";
	this->_status[404] = "Not Found";
	this->_status[405] = "Method Not Allowed";
	this->_status[411] = "Length Required";
	this->_status[413] = "Request Entity Too Large";
	this->_status[500] = "Internal Server Error";
	this->_status[501] = "Not Implemented";
	this->_status[505] = "HTTP Version not supported";
	this->_fileError[404] = "/error/404.html";
	this->_fileError[405] = "/error/405.html";
	this->_fileError[500] = "/error/500.html";
}

HttpResponse::~HttpResponse()
{
}

HttpResponse::HttpResponse(HttpResponse const &rhs)
{
	*this = rhs;
}

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs)
{
	if (this == &rhs)
		return *this;
	// do something
	return *this;
}

void HttpResponse::setConnection(std::string const &connection)
{
	this->_connection = connection;
}

void	HttpResponse::setBody(std::string const &body)
{
	this->_body = body;
}

void	HttpResponse::setServername(std::string const &servername)
{
	this->_serverName = servername;
}

void HttpResponse::setPath(std::string const &path)
{
	this->_path = path;
}

void HttpResponse::setPort(std::string const &port)
{
	this->_port = port;
}

void HttpResponse::setMethod(std::string const &method)
{
	this->_method = method;
}

void HttpResponse::setFileName(std::string const &filename)
{
	this->_filename = filename;
}

void HttpResponse::_checkFile()
{
	std::ifstream ifs(this->_fileResponse);

	if (ifs.good())
		ifs.close();
	else
		throw(404);
}

void	HttpResponse::_checkAutoIndex()
{
	std::string autoindex = this->_config->getServConfigVal(this->_config_ser, "autoindex");

	if (autoindex.length() > 0 && autoindex.compare("on") == 0)
		this->_autoIndex = 1;
}

void HttpResponse::setConfig(ConfigFileHandle *config)
{
	this->_config = config;
}

void HttpResponse::_setHeader(std::string const &key, std::string const &value)
{
	this->_header[key] = value;
}

int HttpResponse::_checkPort()
{
	unsigned int amount_ser = this->_config->getAmountServConfig();
	for (int i = 0; i < amount_ser; i++)
	{
		if (this->_port.compare(this->_config->getServConfigVal(i, "listen")) == 0)
		{
			this->_config_ser = i;
			break;
		}
	}
	return (this->_config_ser);
}

int	HttpResponse::_checkPath()
{
	this->_config_location = (this->_config_ser > -1) ? this->_config->getServConfigVal(this->_config_ser, "location " + this->_path) : "";

	if (this->_config_location.length() == 0) {
		std::string 				tmpPath = this->_path;
		std::size_t 				found = tmpPath.find_last_of("/");
		while (found != std::string::npos) {
			tmpPath = tmpPath.substr(0, found);
			this->_config_location = (this->_config_ser > -1) ? this->_config->getServConfigVal(this->_config_ser, "location " + tmpPath + "/") : "";
			if (this->_config_location.length() > 0)
				break ;
			found = tmpPath.find_last_of("/");
		}
	}

	return this->_config_location.length();
}

void HttpResponse::_checkReturn()
{
	std::string re = this->_setConfigCondition("return");
	std::vector<std::string> arr_return;

	if (re != "return")
	{
		arr_return = this->_spiltString(re, " ");
		this->_return = arr_return[1];
		throw(301);
	}
}

void HttpResponse::_setConfig()
{
	std::string condition = this->_config_location;
	std::string delimiter = ";";
	size_t pos = 0;
	std::string token;

	while ((pos = condition.find(delimiter)) != std::string::npos)
	{
		token = condition.substr(0, pos);
		while (token.find(" ") == 0)
			token.erase(0, 1);
		this->_config_condition.push_back(token);
		condition.erase(0, pos + delimiter.length());
	}
	this->_config_condition.push_back(condition);
}

std::string HttpResponse::_setConfigCondition(std::string const &nameCondition)
{
	std::string condition = nameCondition;

	for (auto value : this->_config_condition)
	{
		if (condition == value.substr(0, condition.length()))
			return value;
	}

	return condition;
}

std::vector<std::string> HttpResponse::_spiltString(std::string &str, std::string const &delim)
{
	size_t pos = 0;
	std::string token;
	std::vector<std::string> arr_return;

	while ((pos = str.find(delim)) != std::string::npos)
	{
		token = str.substr(0, pos);
		while (token.find(" ") == 0)
			token.erase(0, 1);
		arr_return.push_back(token);
		str.erase(0, pos + delim.length());
	}
	arr_return.push_back(str);

	return arr_return;
}

void HttpResponse::_checkMethod()
{
	std::string method = this->_setConfigCondition("allow_method");
	std::vector<std::string> allow_method;

	if (method != "allow_method")
	{
		allow_method = this->_spiltString(method, " ");
		for (auto value : allow_method)
		{
			if (value == this->_method)
				return;
		}
		throw(405);
	}
}

void HttpResponse::_setRootPath()
{
	std::string 				path = this->_setConfigCondition("root");
	std::string 				tmp;
	std::vector<std::string>	config_path;
	std::vector<std::string>	filename_delete;

	if (path.length())
	{
		config_path = this->_spiltString(path, " ");
		this->_config_root = config_path[1];
		if (this->_method == "DELETE") {
			tmp = this->_path;
			filename_delete = this->_spiltString(tmp, "/");
			this->_filenameDelete = filename_delete[filename_delete.size() - 1];
		}
		return ;
	}
	throw(500);
}

void printVector(const std::vector<std::string> &vec)
{
	std::vector<std::string>::const_iterator iter; // Iterator for vector

	for (iter = vec.begin(); iter != vec.end(); ++iter)
	{
		std::cout << YEL << *iter << reset << std::endl; // Print each string element
	}
}

std::string HttpResponse::_searchIndex(std::string const &pathFile)
{
	std::string index = this->_setConfigCondition("index");
	std::vector<std::string> arr_index;

	if (index != "index")
	{
		arr_index = this->_spiltString(index, " ");
		for (auto value : arr_index)
		{
			if (value == "index")
				continue;
			std::ifstream ifs(pathFile + value);
			if (ifs.good())
			{
				ifs.close();
				return (pathFile + value);
			}
		}
	}

	throw(404);
}

void HttpResponse::_setFileResponse(std::string const &pathFile, std::string const &rootPath)
{
	if (rootPath.compare("/") == 0)
	{
		this->_fileResponse = this->_searchIndex(pathFile);
	}
	else
	{
		this->_fileResponse = pathFile;
	}

	this->_checkFile();
}

void HttpResponse::_setErrorPage()
{
	std::string error_page = "error_page";
	std::vector<std::string> config_path;

	for (auto value : this->_config_condition)
	{
		if (error_page == value.substr(0, error_page.length()))
		{
			config_path = this->_spiltString(value, " ");
			this->_fileError[std::stoi(config_path[1])] = config_path[2];
		}
	}
}

void HttpResponse::_setCGI()
{
	std::string tmp;
	std::vector<std::string> arr_cgi;
	std::vector<std::string> arr_cgi_program;

	this->_config_cgi = (this->_config_ser > -1) ? this->_config->getServConfigVal(this->_config_ser, "location_back /cgi_bins/.py") : "";

	if (this->_config_cgi.length() > 0)
	{
		tmp = this->_config_cgi;
		arr_cgi = this->_spiltString(tmp, ";");
		for (auto value : arr_cgi)
		{
			if (value.substr(0, 6) == "is_cgi" && value.substr(7, 9) == "on")
				this->_checkCGI = 1;
			if (value.substr(0, 4) == "root")
				this->_config_cgi_path = value.substr(5, value.length() - 5);
			if (value.substr(0, 12) == "cgi_executor")
			{
				this->_config_cgi_ext = value.substr(13, value.length() - 12);
				std::size_t found = this->_config_cgi_ext.find_last_of("/");
				this->_config_cgi_program = this->_config_cgi_ext.substr(found + 1);
			}
		}
	}
}

std::string HttpResponse::_setENVArgv(std::string const &name, std::string const &value)
{
	return (name + "=" + value);
}

std::string HttpResponse::_setArgvPath()
{
	std::string name_cgi = "";

	if (this->_method == "GET" && this->_statusCode == 404 && this->_autoIndex == 1 && this->_path != "/favicon.ico")
		name_cgi = "/autoindex.py";
	else if (this->_method == "POST" && this->_statusCode != 405)
		name_cgi = "/post.py";
	else if (this->_method == "DELETE")
		name_cgi = "/delete.py";
	else if (this->_method == "GET" && this->_statusCode == 301)
		name_cgi = "/redirect.py";
	else if (this->_method == "GET" || this->_statusCode != 200)
		name_cgi = "/get.py";

	return this->_config_cgi_path + name_cgi;
}

void HttpResponse::_setContentType()
{
	std::size_t found = this->_path.find_last_of(".");
	std::string extension = this->_path.substr(found + 1);

	if (extension == "/" || extension == "html")
	{
		this->_contentType = "text/html";
	} else {
		if (this->_autoIndex == 1)
			this->_contentType = "text/html";
		else
			this->_contentType = "text/plain";
	}
}

std::string HttpResponse::_setResponseStream()
{
	std::string contentRes = "";
	std::ostringstream resStream;
	std::map<std::string, std::string>::iterator it;

	if (this->_checkCGI) {
		int 			n = 0;
		int				fd[2];
		int				post[2];
		char 			buffer[1024];
		pid_t			pid;
		std::string		path = this->_setArgvPath();
		char *const		argv[] = {
							const_cast<char *>(this->_config_cgi_program.data()), 
							const_cast<char *>(path.data()), 
							NULL
						};
		std::string		filename = this->_setENVArgv("FILENAME", this->_fileResponse);
		std::string		statusCode = this->_setENVArgv("STATUS_CODE", std::to_string(this->_statusCode));
		std::string		statusMessage = this->_setENVArgv("STATUS_MESSAGE", this->_status[this->_statusCode]);
		std::string		hostname = this->_setENVArgv("HOSTNAME", this->_serverName);
		std::string		port = this->_setENVArgv("PORT", this->_port);
		std::string		argvPath = this->_setENVArgv("PATH", this->_path);
		std::string		root_path = this->_setENVArgv("ROOT_PATH", this->_config_root);
		std::string		url = this->_setENVArgv("URL", this->_url);
		std::string		connection = this->_setENVArgv("CONNECTION", this->_connection);
		std::string		contentType = this->_setENVArgv("CONTENT_TYPE", this->_contentType);
		std::string		body = this->_setENVArgv("BODY", this->_body);
		std::string		filenameDelete = this->_setENVArgv("FILENAME_DELETE", this->_filenameDelete);
		std::string		upfilename = this->_setENVArgv("UPLOAD_FILENAME", this->_filename);
		std::string		redirect = this->_setENVArgv("REDIRECT", this->_return);

		char			*envp[] = {
							const_cast<char *>(filename.data()),
							const_cast<char *>(statusCode.data()),
							const_cast<char *>(statusMessage.data()),
							const_cast<char *>(hostname.data()),
							const_cast<char *>(port.data()),
							const_cast<char *>(argvPath.data()),
							const_cast<char *>(connection.data()),
							const_cast<char *>(contentType.data()),
							const_cast<char *>(url.data()),
							const_cast<char *>(root_path.data()),
							const_cast<char *>(body.data()),
							const_cast<char *>(filenameDelete.data()),
							const_cast<char *>(upfilename.data()),
							const_cast<char *>(redirect.data()),
							NULL
						};
		const char		*path_cmd = this->_config_cgi_ext.c_str();

		pipe(fd);
		if (this->_method == "POST") {
			pipe(post);
			write(post[1], this->_body.c_str(), this->_body.length());
		}
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			if (this->_method == "POST") {
				close(post[1]);
				dup2(post[0], 0);
				close(post[0]);
			}
			execve(path_cmd, argv, envp);
		}
		else
		{
			if (this->_method == "POST") {
				close(post[0]);
				close(post[1]);
			}
			close(fd[1]);
			waitpid(pid, NULL, 0);
			contentRes = "";
			while ((n = read(fd[0], buffer, 1024)) > 0)
			{
				buffer[n] = '\0';
				contentRes += buffer;
			}
			close(fd[0]);
		}
	}
	else
	{
		std::ifstream ifs(this->_fileResponse);
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		contentRes = content;
		this->_setHeader("Content-Length:", std::to_string(contentRes.length()));
		if (this->_statusCode != 301) {
			this->_setHeader("Content-Type:", "text/html");
			this->_setHeader("Location-Header:", "TH");
		} else {
			this->_setHeader("Location:", this->_return);
		}

		resStream << "HTTP/1.1 " << this->_statusCode << " " << this->_status[this->_statusCode] << "\r\n";
		for (it = this->_header.begin(); it != this->_header.end(); it++)
		{
			resStream << it->first << " " << it->second << "\r\n";
		}
		resStream << "\r\n";
	}
	resStream << contentRes;

	return resStream.str();
}

std::string HttpResponse::returnResponse()
{
	try
	{
		if (this->_checkPort() > -1 && this->_checkPath())
		{
			this->_checkAutoIndex();
			this->_setConfig();
			this->_setErrorPage();
			this->_setCGI();
			this->_url = "http://" + this->_serverName + ":" + this->_port + this->_path;
			this->_checkReturn();
			this->_checkMethod();
			this->_setRootPath();
			this->_setContentType();
			this->_setFileResponse(this->_config_root + this->_path, this->_path);
			this->_statusCode = 200;
		} else {
			throw (404);
		}
	} catch (int status) {
		this->_statusCode = status;
		this->_fileResponse = this->_config_root + this->_fileError[status];
	}

	return (this->_setResponseStream());
}
