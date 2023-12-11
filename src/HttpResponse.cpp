#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	this->_statusCode = 200;
	this->_checkCGI = 0;
	this->_autoIndex = 0;
	this->_config_cgi_path = "";
	this->_url = "";
	this->_filenameDelete = "";
	this->_return = "";

	// default request
	this->_body = "";
	this->_config = NULL;
	this->_connection = "";
	this->_filename = "";
	this->_method = "";
	this->_path = "";
	this->_port = "";
	this->_serverName = "";
	// default serve
	this->_config_ser = -1;
	this->_config_root = "./page"; // default root
	this->_config_location = "";

	// default status
	this->_status[200] = "OK";
	this->_status[201] = "Created";
	this->_status[301] = "Moved Permanently";
	this->_status[400] = "Bad Request";
	this->_status[404] = "Not Found";
	this->_status[405] = "Method Not Allowed";
	this->_status[413] = "Request Entity Too Large";
	this->_status[500] = "Internal Server Error";

	// default error page
	this->_fileError[400] = "/error/400.html";
	this->_fileError[404] = "/error/404.html";
	this->_fileError[405] = "/error/405.html";
	this->_fileError[413] = "/error/413.html";
	this->_fileError[500] = "/error/500.html";
}

HttpResponse::~HttpResponse()
{
}

/////////////
// PRIVATE //
/////////////

// utils

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

// set ARGV and ENV for CGI

std::string HttpResponse::_setArgvPath()
{
	std::string name_cgi = "";

	if (this->_method == "GET" && this->_statusCode == 404 && this->_autoIndex == 1 && this->_path != "/favicon.ico")
		name_cgi = "autoindex";
	else if (this->_method == "POST" && this->_statusCode != 405 && this->_statusCode != 413)
		name_cgi = "post";
	else if (this->_method == "DELETE")
		name_cgi = "delete";
	else if (this->_method == "GET" && this->_statusCode == 301)
		name_cgi = "redirect";
	else if (this->_method == "GET" || this->_statusCode != 200)
		name_cgi = "get";

	return this->_config_cgi_path + "/" + name_cgi + "." + this->_suffixCGI;
}

std::string HttpResponse::_setENVArgv(std::string const &name, std::string const &value)
{
	return (name + "=" + value);
}

// setting serve

int HttpResponse::_checkPort()
{
	unsigned int amount_ser = this->_config->getAmountServConfig();

	for (unsigned int i = 0; i < amount_ser; i++) {
		if (this->_port.compare(this->_config->getServConfigVal(i, "listen")) == 0) {
			this->_config_ser = i;
			break;
		}
	}

	return (this->_config_ser);
}

int HttpResponse::_checkPath()
{
	std::string 				tmp = this->_path;
	std::vector<std::string>	path_trim = this->_spiltString(tmp, ":");

	if (path_trim.size() > 1)
		path_trim[0] += "/";
	this->_config_location = this->_config->getServConfigVal(this->_config_ser, "location " + path_trim[0]);

	if (this->_config_location.length() == 0)
	{
		std::string tmpPath = path_trim[0];
		std::size_t found = tmpPath.find_last_of("/");

		while (found != std::string::npos)
		{
			tmpPath = tmpPath.substr(0, found);
			this->_config_location = this->_config->getServConfigVal(this->_config_ser, "location " + tmpPath + "/");
			if (this->_config_location.length() > 0)
				break;
			found = tmpPath.find_last_of("/");
		}
	}

	return this->_config_location.length();
}

////////////
// PUBLIC //
////////////

void HttpResponse::setBody(std::string const &body)
{
	this->_body = body;
}

void HttpResponse::setConfig(ConfigFileHandle *config)
{
	this->_config = config;
}

void HttpResponse::setConnection(std::string const &connection)
{
	this->_connection = connection;
}

void HttpResponse::setFileName(std::string const &filename)
{
	this->_filename = filename;
}

void HttpResponse::setMethod(std::string const &method)
{
	this->_method = method;
}

void HttpResponse::setPath(std::string const &path)
{
	this->_path = path;
}

void HttpResponse::setPort(std::string const &port)
{
	this->_port = port;
}

void HttpResponse::setServername(std::string const &servername)
{
	this->_serverName = servername;
}

std::string HttpResponse::returnResponse()
{
	this->printSetForRequest();
	try
	{
		if (this->_checkPort() > -1 && this->_checkPath()) {
			this->_checkAutoIndex();
			this->_setConfig();
			this->_setErrorPage();
			this->_setCGI();
			this->_url = "http://" + this->_serverName + ":" + this->_port + this->_path;
			this->_checkReturn();
			this->_checkMethod();
			this->_setRootPath();
			this->_setContentType();
			if (this->_method == "POST")
			{
				this->_checkBodySize();
			}
			this->_setFileResponse(this->_config_root + this->_path, this->_path);
			this->_statusCode = 200;
		}
		else
			throw(404);
	} catch (int status) {
		this->_statusCode = status;
		this->_fileResponse = this->_config_root + this->_fileError[status];
	}

	return (this->_setResponseStream());
}

//////////
// TEST //
//////////


void HttpResponse::printVector(const std::vector<std::string> &vec)
{
	std::vector<std::string>::const_iterator iter;

	for (iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << std::endl;
	}
}

void HttpResponse::printSetForRequest()
{
	std::cout << "this->_body: " << this->_body << std::endl;
	std::cout << "this->_connection: " << this->_connection << std::endl;
	std::cout << "this->_filename: " << this->_filename << std::endl;
	std::cout << "this->_method: " << this->_method << std::endl;
	std::cout << "this->_path: " << this->_path << std::endl;
	std::cout << "this->_port: " << this->_port << std::endl;
	std::cout << "this->_serverName: " << this->_serverName << std::endl;
}









void HttpResponse::_checkFile()
{
	std::ifstream ifs(this->_fileResponse);

	if (ifs.good())
		ifs.close();
	else {
std::cout << "(_checkFile) this->_fileResponse: " << this->_fileResponse << std::endl;
		throw(404);
	}
}

void HttpResponse::_checkAutoIndex()
{
	std::string autoindex = this->_config->getServConfigVal(this->_config_ser, "autoindex");

	if (autoindex.length() > 0 && autoindex.compare("on") == 0)
		this->_autoIndex = 1;
}

void HttpResponse::_setHeader(std::string const &key, std::string const &value)
{
	this->_header[key] = value;
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

void HttpResponse::_checkMethod()
{
	// std::cout << YEL << "In _checkMethod()" << reset << std::endl;
	// std::cout << YEL << "method: " << this->_method << reset << std::endl;
	std::string method = this->_setConfigCondition("allow_method");
	std::vector<std::string> allow_method;

	if (method != "allow_method")
	{
		allow_method = this->_spiltString(method, " ");
		for (auto value : allow_method)
		{
			// std::cout << YEL << "allow method: " << value << reset << std::endl;
			if (value == this->_method)
				return;
		}
		// std::cout << YEL << "Get throw 405" << reset << std::endl;
		throw(405);
	}
}

void HttpResponse::_setRootPath()
{
	std::string path = this->_setConfigCondition("root");
	std::string tmp;
	std::vector<std::string> config_path;
	std::vector<std::string> filename_delete;

	if (path.length())
	{
		config_path = this->_spiltString(path, " ");
		this->_config_root = config_path[1];
		if (this->_method == "DELETE")
		{
			tmp = this->_path;
			filename_delete = this->_spiltString(tmp, "/");
			this->_filenameDelete = filename_delete[filename_delete.size() - 1];
		}
		return;
	}
	throw(500);
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
		std::cout << "if" << std::endl;
	}
	else
	{
		std::cout << "else" << std::endl;
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

	this->_config_cgi = "";
	if (this->_config_ser > -1) {
		this->_config_cgi = this->_config->getServConfigVal(this->_config_ser, "location_back /cgi-bins/.py");
		this->_suffixCGI = "py";
	}
	if (this->_config_cgi.length() == 0) {
		this->_config_cgi = this->_config->getServConfigVal(this->_config_ser, "location_back /cgi-bins/.php");
		this->_suffixCGI = "php";
	}

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

void HttpResponse::_setContentType()
{
	std::size_t found = this->_path.find_last_of(".");
	std::string extension = this->_path.substr(found + 1);

	if (extension == "/" || extension == "html")
	{
		this->_contentType = "text/html";
	}
	else
	{
		if (this->_autoIndex == 1)
			this->_contentType = "text/html";
		else
			this->_contentType = "text/html";
	}
}

void HttpResponse::_checkBodySize()
{
	std::string body_size = this->_setConfigCondition("client_max_body_size");
	std::vector<std::string> arr_body_size;

	if (body_size != "client_max_body_size")
	{
		arr_body_size = this->_spiltString(body_size, " ");
		if (std::stoi(arr_body_size[1]) < this->_body.length())
			throw(413);
	}
}

std::string HttpResponse::_setResponseStream()
{
	std::string contentRes = "";
	std::ostringstream resStream;
	std::map<std::string, std::string>::iterator it;

	if (this->_checkCGI)
	{
		int n = 0;
		int fd[2];
		int post[2];
		char buffer[1024];
		pid_t pid;
		std::string path = this->_setArgvPath();
		char *const argv[] = {
			const_cast<char *>(this->_config_cgi_program.data()),
			const_cast<char *>(path.data()),
			NULL};
		std::string filename = this->_setENVArgv("FILENAME", this->_fileResponse);
		std::string statusCode = this->_setENVArgv("STATUS_CODE", std::to_string(this->_statusCode));
		std::string statusMessage = this->_setENVArgv("STATUS_MESSAGE", this->_status[this->_statusCode]);
		std::string hostname = this->_setENVArgv("HOSTNAME", this->_serverName);
		std::string port = this->_setENVArgv("PORT", this->_port);
		std::string argvPath = this->_setENVArgv("PATH", this->_path);
		std::string root_path = this->_setENVArgv("ROOT_PATH", this->_config_root);
		std::string url = this->_setENVArgv("URL", this->_url);
		std::string connection = this->_setENVArgv("CONNECTION", this->_connection);
		std::string contentType = this->_setENVArgv("CONTENT_TYPE", this->_contentType);
		std::string body = this->_setENVArgv("BODY", this->_body);
		std::string filenameDelete = this->_setENVArgv("FILENAME_DELETE", this->_filenameDelete);
		std::string upfilename = this->_setENVArgv("UPLOAD_FILENAME", this->_filename);
		std::string redirect = this->_setENVArgv("REDIRECT", this->_return);
		char *envp[] = {
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
			NULL};
		const char *path_cmd = this->_config_cgi_ext.c_str();

		pipe(fd);
		if (this->_method == "POST")
		{
			pipe(post);
			write(post[1], this->_body.c_str(), this->_body.length());
		}
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			if (this->_method == "POST")
			{
				// std::cout << YEL << "In child process of POST" << reset << std::endl;
				close(post[1]);
				dup2(post[0], 0);
				close(post[0]);
			}
			execve(path_cmd, argv, envp);
		}
		else
		{
			if (this->_method == "POST")
			{
				std::cout << YEL << "Close fd of POST" << reset << std::endl;
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
		if (this->_method == "DELETE")
		{
			std::string del_filename = this->_fileResponse;
			remove(del_filename.c_str());
			contentRes = this->_filenameDelete + " has been deleted successfully.";
		}
		else if (this->_method == "POST" && this->_statusCode != 405 && this->_statusCode != 413)
		{
			struct stat statbuf;
			std::string path_upload = this->_config_root + this->_path;
			std::string path_file = path_upload + this->_filename;
			std::ifstream ifs;
			if (stat(path_upload.c_str(), &statbuf) != 0)
				mkdir(path_upload.c_str(), 0764);
			ifs.open(path_file, std::ifstream::in);
			if (ifs.good() == false)
			{
				ifs.close();
				std::ofstream ofs(path_file);
				ofs << this->_body;
				ofs.close();
				this->_statusCode = 201;
				contentRes = this->_filename + " has been uploaded successfully.";
			}
			else
			{
				ifs.close();
				this->_statusCode = 400;
				contentRes = this->_filename + " already exists.";
			}
		}
		else
		{
			std::ifstream ifs(this->_fileResponse);
			std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			contentRes = content;
		}

		this->_setHeader("Content-Length:", std::to_string(contentRes.length()));
		if (this->_statusCode != 301)
		{
			this->_setHeader("Content-Type:", "text/html");
			this->_setHeader("Location-Header:", "TH");
		}
		else
		{
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
