#ifndef HTTPREPONSE_HPP
#define HTTPREPONSE_HPP

#include "Http.hpp"
#include "ConfigFileHandle.hpp"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>

class HttpResponse : public Http
{
	private:
		int					_statusCode;
		int					_checkCGI;
		int					_autoIndex;
		std::string 		_fileResponse;
		std::string 		_serverName;
		std::string 		_path;
		std::string 		_port;
		std::string			_method;
		std::string			_contentType;
		std::string			_connection;
		std::string			_url;
		std::string			_body;
		std::string			_filenameDelete;
		std::string			_filename;
		std::string			_return;
		ConfigFileHandle	*_config;
		std::map<int, std::string>			_status;
		std::map<int, std::string>			_fileError;
		std::map<std::string, std::string>	_header;
		// read from ConfigFileHandle
		int					_config_ser;
		std::string			_config_location;
		std::string			_config_root;
		std::string			_config_cgi;
		std::string			_config_cgi_path;
		std::string			_config_cgi_ext;
		std::string			_config_cgi_program;
		std::vector<std::string>	_config_condition;
		// set ENV and ARGV
		std::string			_setArgvPath();
		std::string			_setENVArgv(std::string const &name, std::string const &value);
		// func check and set before send response
		int					_checkPort();
		int					_checkPath();
		void				_checkReturn();
		void				_setHeader(std::string const &key, std::string const &value);
		void				_setConfig();
		void				_checkAutoIndex();
		void				_checkMethod();
		void				_setRootPath();
		void				_setFileResponse(std::string const &pathFile, std::string const &rootPath);
		void				_setErrorPage();
		void				_setCGI();
		void				_setContentType();
		void				_checkFile();
		void				_checkBodySize();
		std::string			_setConfigCondition(std::string const &nameCondition);
		std::string			_searchIndex(std::string const &pathFile);
		std::string			_setResponseStream();
		std::vector<std::string>	_spiltString(std::string &str, std::string const &delim);

	public:
		HttpResponse();
		~HttpResponse();
		HttpResponse(HttpResponse const &rhs);
		HttpResponse	&operator=(HttpResponse const &rhs);

		void			setServername(std::string const &servername);
		void			setPath(std::string const &path);
		void			setPort(std::string const &port);
		void			setMethod(std::string const &method);
		void			setConfig(ConfigFileHandle *config);
		void			setConnection(std::string const &connection);
		void			setBody(std::string const &body);
		void			setFileName(std::string const &filename);
		std::string		returnResponse();
};

#endif