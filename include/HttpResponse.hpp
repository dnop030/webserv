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
		// default request
		std::string			_body;
		ConfigFileHandle	*_config;
		std::string			_connection;
		std::string			_filename;
		std::string			_method;
		std::string 		_path;
		std::string 		_port;
		std::string 		_serverName;
		// default status
		std::map<int, std::string>	_status;
		// default error page
		std::map<int, std::string>	_fileError;
		// default response
		int					_statusCode;
		int					_autoIndex;
		std::string			_return;
		std::string			_filenameDD; // filename for (delete or download)
		std::string			_contentType;
		std::string 		_fileResponse;
		std::map<std::string, std::string>	_all_config;
		std::map<std::string, std::string>	_header;
		// default config serve
		int					_config_ser;
		std::string			_config_root;
		std::string			_config_location;
		std::string			_config_suffix_cgi;
		std::map<std::string, std::string>	_config_cgi;
		// utils
		int							_checkFileExits(std::string const &file_name);
		void						_checkFile();
		void						_setHeader(std::string const &key, std::string const &value);
		std::string					_setENVArgv(std::string const &name, std::string const &value);
		std::string					_searchIndex(std::string const &pathFile);
		std::vector<std::string>	_spiltString(std::string &str, std::string const &delim);
		// setting serve
		int					_checkPort();
		int					_checkPath();
		int					_checkAutoIndex();
		void				_setConfig();
		void				_checkReturn();
		void				_setRootPath();
		void				_setCGI();
		void				_checkAllowMethod();
		void				_checkBodySize();
		void				_setFileResponse(std::string const &pathFile, std::string const &rootPath);
		void 				_checkFilenameDD();
		std::string			_methodDelete();
		std::string			_methodPost();
		std::string			_setArgvPath();		
		std::string			_cgi();
		std::string			_setResponseStream();

	public:
		HttpResponse();
		~HttpResponse();
		// HttpResponse(HttpResponse const &rhs);
		// HttpResponse	&operator=(HttpResponse const &rhs);

		// SETTER
		void			setBody(std::string const &body);
		void			setConfig(ConfigFileHandle *config);
		void			setConnection(std::string const &connection);
		void			setFileName(std::string const &filename);
		void			setMethod(std::string const &method);
		void			setPath(std::string const &path);
		void			setPort(std::string const &port);
		void			setServername(std::string const &servername);

		std::string		returnResponse();

		// test del
		void printVector(const std::vector<std::string> &vec);
		void printSetForRequest();
		void printSetForResponse();
		void printSetForCgi(const std::string &name_cgi);
};

#endif