#include "HttpResponse.hpp"

HttpResponse::HttpResponse()
{
	// default request
	this->_body = "";
	this->_config = NULL;
	this->_connection = "";
	this->_filename = "";
	this->_method = "";
	this->_path = "";
	this->_port = "";
	this->_serverName = "";
	// default response
	this->_statusCode = 200;
	this->_autoIndex = 0;
	this->_return = "";
	this->_filenameDD = "";
	this->_contentType = "text/html";
	this->_fileResponse = "";
	this->_all_config.clear();
	this->_header.clear();
	// default config serve
	this->_config_ser = -1;
	this->_config_root = "./page"; // default root
	this->_config_location = "";
	this->_config_suffix_cgi = "";
	this->_config_cgi.clear();

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
	this->_fileError[301] = "/error/301.html";
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

int HttpResponse::_checkFileExits(const std::string &file_name)
{
	std::ifstream ifs(file_name);

	if (ifs.good()) {
		ifs.close();
		return (1);
	}

	return (0);
}

void HttpResponse::_checkFile()
{
	std::ifstream ifs(this->_fileResponse);

	(ifs.good()) ? ifs.close() : throw(404);
}

void HttpResponse::_setHeader(std::string const &key, std::string const &value)
{
	this->_header[key] = value;
}

std::string HttpResponse::_setENVArgv(std::string const &name, std::string const &value)
{
	return (name + "=" + value);
}

std::string HttpResponse::_searchIndex(std::string const &pathFile)
{
	auto index = this->_all_config.find("index");

	if (index != this->_all_config.end()) {
		std::vector<std::string> arr_index = this->_spiltString(index->second, " ");

		for (auto value : arr_index) {
			if (value.length() == 0)
				continue;
			std::ifstream ifs(pathFile + value);
			if (ifs.good()) {
				ifs.close();
				return (pathFile + value);
			}
		}
	}

	throw(404);
}

std::vector<std::string> HttpResponse::_spiltString(std::string &str, std::string const &delim)
{
	size_t 						pos = 0;
	std::string 				token;
	std::vector<std::string>	arr_return;

	while ((pos = str.find(delim)) != std::string::npos) {
		token = str.substr(0, pos);
		while (token.find(" ") == 0)
			token.erase(0, 1);
		arr_return.push_back(token);
		str.erase(0, pos + delim.length());
	}
	arr_return.push_back(str);

	return arr_return;
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

	if (this->_config_location.length() == 0) {
		std::string tmpPath = path_trim[0];
		std::size_t found = tmpPath.find_last_of("/");

		while (found != std::string::npos) {
			tmpPath = tmpPath.substr(0, found);
			this->_config_location = this->_config->getServConfigVal(this->_config_ser, "location " + tmpPath + "/");
			if (this->_config_location.length() > 0)
				break;
			found = tmpPath.find_last_of("/");
		}
	}

	return this->_config_location.length();
}

int HttpResponse::_checkAutoIndex()
{
	std::string autoindex = this->_config->getServConfigVal(this->_config_ser, "autoindex");

	if (autoindex.length() > 0 && autoindex.compare("on") == 0)
		this->_autoIndex = 1;

	return (1);
}

void HttpResponse::_setConfig()
{
	std::vector<std::string> arr_config_condition = this->_spiltString(this->_config_location, ";");
	std::vector<std::string> tmp;

	for (auto value : arr_config_condition) {
		if (value.length() == 0)
			continue;
		tmp = this->_spiltString(value, " ");
		if (tmp[0].compare("error_page") == 0)
			this->_fileError[std::stoi(tmp[1])] = tmp[2];
		else if (tmp[0].compare("allow_method") == 0 || tmp[0].compare("index") == 0) {
			std::string tmp_val = "";
			for(int i = tmp.size() - 1; i > 0; i--)
				tmp_val += tmp[i] + " ";
			this->_all_config[tmp[0]] = tmp_val;
		} else
			this->_all_config[tmp[0]] = tmp[1];
	}
}

void HttpResponse::_checkReturn()
{
	auto re = this->_all_config.find("return");

	if (re != this->_all_config.end()) {
		this->_return = re->second;
		throw(301);
	}
}

void HttpResponse::_setRootPath()
{
	auto root = this->_all_config.find("root");

	if (root == this->_all_config.end())
		throw(500);
	this->_config_root = root->second;
	if ((this->_method == "DELETE")|| (this->_method == "GET" && this->_path.find("?file=") != std::string::npos)) {
		std::vector<std::string>  filename = this->_spiltString(this->_path, "=");
		
		this->_filenameDD = filename[filename.size() - 1];
	}
}

void HttpResponse::_setCGI()
{
	std::string 				config_cgi = this->_config->getServConfigVal(this->_config_ser, "location_back /cgi-bins/.py");
	std::vector<std::string>	arr_cgi;
	std::vector<std::string>	tmp;

	if (config_cgi.length() > 0) {
		this->_config_suffix_cgi = "py";
	} if (config_cgi.length() == 0) {
		config_cgi = this->_config->getServConfigVal(this->_config_ser, "location_back /cgi-bins/.php");
		this->_config_suffix_cgi = "php";
	}

	if (config_cgi.length() > 0) {
		arr_cgi = this->_spiltString(config_cgi, ";");

		for (auto value : arr_cgi) {
			if (value.length() == 0)
				continue;
			tmp = this->_spiltString(value, " ");
			if (tmp[0].compare("cgi_executor") == 0) {
				this->_config_cgi["path_cmd"] = tmp[1];
				std::size_t found = tmp[1].find_last_of("/");
				this->_config_cgi["program"] = tmp[1].substr(found + 1, tmp[1].length() - found);
			} else
				this->_config_cgi[tmp[0]] = tmp[1];
		}
	}
}

void HttpResponse::_checkAllowMethod()
{
	auto method = this->_all_config.find("allow_method");

	if (method != this->_all_config.end()) {
		if (method->second.find(this->_method) == std::string::npos)
			throw(405);
	}
}

void HttpResponse::_checkBodySize()
{
	auto body_size = this->_all_config.find("client_max_body_size");

	if (this->_method == "POST" && body_size != this->_all_config.end() && std::stoi(body_size->second) < this->_body.length())
		throw(413);
}

void HttpResponse::_setFileResponse(std::string const &pathFile, std::string const &rootPath)
{
	std::string full_path = pathFile + rootPath;

	this->_fileResponse = (rootPath.compare("/") == 0) ? this->_searchIndex(full_path) : full_path;
	if (this->_filenameDD.length() == 0 && this->_method != "POST")
		this->_checkFile();
}

void HttpResponse::_checkFilenameDD() 
{
	if (this->_filenameDD.length() > 0) {
		this->_fileResponse = this->_config_root + ":" + this->_port + "/" + this->_filenameDD;
		if (this->_checkFileExits(this->_fileResponse)) {
			this->_contentType = "application/octet-stream";
			this->_setHeader("Content-Disposition:", "attachment; filename=" + this->_filenameDD);
		} else {
			this->_statusCode = 404;
			this->_fileResponse = "./page" + this->_fileError[404];
		}
	}
}

std::string HttpResponse::_methodDelete()
{
	std::string del_filename = this->_config_root + ":" + this->_port + "/" + this->_filenameDD;

	if (this->_checkFileExits(del_filename)) {
		remove(del_filename.c_str());

		return this->_filenameDD + " has been deleted successfully.";
	} else
		this->_statusCode = 404;

	return "";
}

std::string HttpResponse::_methodPost()
{
	struct stat 	statbuf;
	std::string		path_upload = this->_config_root + ":"+ this->_port;
	std::string		path_file = path_upload + "/" + this->_filename;
	std::ifstream	ifs;

	if (stat(path_upload.c_str(), &statbuf) != 0)
		mkdir(path_upload.c_str(), 0764);

	ifs.open(path_file, std::ifstream::in);
	if (ifs.good()) {
		ifs.close();
		this->_statusCode = 400;

		return "";
	}
	std::ofstream ofs(path_file);
	ofs << this->_body;
	ofs.close();
	this->_statusCode = 201;

	return this->_filename + " has been uploaded successfully.";
}

std::string HttpResponse::_setArgvPath()
{
	std::string name_cgi = "";

	if (this->_statusCode == 404 && this->_autoIndex == 1)
		name_cgi = "autoindex";
	else if (this->_method == "DELETE")
		name_cgi = "delete";
	else if (this->_filenameDD.length() > 0)
		name_cgi = "download";
	else if (this->_method == "POST" && this->_statusCode != 405 && this->_statusCode != 413)
		name_cgi = "post";
	else if (this->_statusCode == 301)
		name_cgi = "redirect";
	else
		name_cgi = "get";

printSetForCgi(name_cgi); // del
	return this->_config_cgi["root"] + "/" + name_cgi + "." + this->_config_suffix_cgi;
}

std::string HttpResponse::_cgi()
{
	int			n = 0;
	int			fd[2];
	int			post[2];
	char		buffer[1024];
	pid_t		pid;
	std::string	path = this->_setArgvPath();
	char *const argv[] = {
					const_cast<char *>(this->_config_cgi["program"].data()),
					const_cast<char *>(path.data()),
					NULL
				};
	std::string	filename = this->_setENVArgv("FILENAME", this->_fileResponse);
	std::string	statusCode = this->_setENVArgv("STATUS_CODE", std::to_string(this->_statusCode));
	std::string	statusMessage = this->_setENVArgv("STATUS_MESSAGE", this->_status[this->_statusCode]);
	std::string	port = this->_setENVArgv("PORT", this->_port);
	std::string	root_path = this->_setENVArgv("ROOT_PATH", this->_config_root);
	std::string	connection = this->_setENVArgv("CONNECTION", this->_connection);
	std::string	contentType = this->_setENVArgv("CONTENT_TYPE", this->_contentType);
	std::string	filenameDelete = this->_setENVArgv("FILENAME_DELETE", this->_filenameDD);
	std::string	filenameDownload = this->_setENVArgv("FILENAME_DOWNLOAD", this->_filenameDD);
	std::string	filenameUpload = this->_setENVArgv("FILENAME_UPLOAD", this->_filename);
	std::string	redirect = this->_setENVArgv("REDIRECT", this->_return);
	std::string	contentRes = "";
	char 		*envp[] = {
					const_cast<char *>(filename.data()),
					const_cast<char *>(statusCode.data()),
					const_cast<char *>(statusMessage.data()),
					const_cast<char *>(port.data()),
					const_cast<char *>(connection.data()),
					const_cast<char *>(contentType.data()),
					const_cast<char *>(root_path.data()),
					const_cast<char *>(filenameDelete.data()),
					const_cast<char *>(filenameDownload.data()),
					const_cast<char *>(filenameUpload.data()),
					const_cast<char *>(redirect.data()),
					NULL
				};
	const char	*path_cmd = this->_config_cgi["path_cmd"].c_str();

	pipe(fd);
	if (this->_method == "POST") {
		pipe(post);
		write(post[1], this->_body.c_str(), this->_body.length());
	}
	pid = fork();
	if (pid == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		if (this->_method == "POST") {
			close(post[1]);
			dup2(post[0], 0);
			close(post[0]);
		}
		execve(path_cmd, argv, envp);
	} else {
		if (this->_method == "POST") {
			close(post[0]);
			close(post[1]);
		}
		close(fd[1]);
		waitpid(pid, NULL, 0);
		while ((n = read(fd[0], buffer, 1024)) > 0) {
			buffer[n] = '\0';
			contentRes += buffer;
		}
		close(fd[0]);
	}

	return contentRes;
}

std::string HttpResponse::_setResponseStream()
{
	std::string contentRes = "";
	std::ostringstream resStream;
	std::map<std::string, std::string>::iterator it;

	if (this->_config_cgi["is_cgi"] == "on")
		contentRes = this->_cgi();
	else {
		if (this->_method == "DELETE")
			contentRes = this->_methodDelete();
		else if (this->_method == "POST" && this->_statusCode != 405 && this->_statusCode != 413)
			contentRes = this->_methodPost();
		else {
			this->_checkFilenameDD();
			std::ifstream ifs(this->_fileResponse);
			std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
			contentRes = content;
		}

		this->_setHeader("Content-Length:", std::to_string(contentRes.length()));
		if (this->_statusCode != 301) {
			this->_setHeader("Connection:", this->_connection);
			this->_setHeader("Content-Type:", this->_contentType);
			this->_setHeader("Location-Header:", "TH");
		} else {
			this->_setHeader("Location:", this->_return);
		}

		resStream << "HTTP/1.1 " << this->_statusCode << " " << this->_status[this->_statusCode] << "\r\n";
		for (it = this->_header.begin(); it != this->_header.end(); it++) {
			resStream << it->first << " " << it->second << "\r\n";
		}
		resStream << "\r\n";
	}
	resStream << contentRes;

	return resStream.str();
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
this->printSetForRequest(); // del
	try {
		if (this->_checkPort() > -1 && this->_checkPath() && this->_checkAutoIndex()) {
			this->_setConfig();
			this->_checkReturn();
			this->_setRootPath();
			this->_setCGI();
			this->_checkAllowMethod();
			this->_checkBodySize();
			this->_setFileResponse(this->_config_root, this->_path);
		}
		else
			throw(404);
	} catch (int status) {
		this->_statusCode = status;
		if (status != 301)
			this->_fileResponse = "./page" + this->_fileError[status];
	}
this->printSetForResponse(); // del
	return (this->_setResponseStream());
}

//////////
// TEST //
//////////

void HttpResponse::printVector(const std::vector<std::string> &vec)
{
	std::vector<std::string>::const_iterator iter;

	for (iter = vec.begin(); iter != vec.end(); ++iter)
	{
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

void HttpResponse::printSetForResponse()
{
	std::cout << "this->_statusCode: " << this->_statusCode << std::endl;
	std::cout << "this->_autoIndex: " << this->_autoIndex << std::endl;
	std::cout << "this->_return: " << this->_return << std::endl;
	std::cout << "this->_filenameDD: " << this->_filenameDD << std::endl;
	std::cout << "this->_contentType: " << this->_contentType << std::endl;
	std::cout << "this->_fileResponse: " << this->_fileResponse << std::endl;
	std::cout << "this->_all_config: " << std::endl;
	for (auto value : this->_all_config)
		std::cout << value.first << " " << value.second << std::endl;
	std::cout << "this->_config_ser: " << this->_config_ser << std::endl;
	std::cout << "this->_config_root: " << this->_config_root << std::endl;
	std::cout << "this->_config_location: " << this->_config_location << std::endl;
	std::cout << "this->_config_suffix_cgi: " << this->_config_suffix_cgi << std::endl;
	std::cout << "this->_config_cgi: " << std::endl;
	for (auto value : this->_config_cgi)
		std::cout << value.first << " " << value.second << std::endl;
}

void HttpResponse::printSetForCgi(const std::string &name_cgi)
{
	std::cout << "this->_config_root: " << this->_config_root << std::endl;
	std::cout << "this->_port: " << this->_port << std::endl;
	std::cout << "this->_filenameDD: " << this->_filenameDD << std::endl;
	std::cout << "this->_filename: " << this->_filename << std::endl;
	std::cout << "this->_name: " << name_cgi << std::endl;
	std::cout << "this->_config_cgi[root]: " << this->_config_cgi["root"] << std::endl;
}