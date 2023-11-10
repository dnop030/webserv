#ifndef HTTPREPONSE_HPP
#define HTTPREPONSE_HPP

#include "Http.hpp"
#include "ConfigFileHandle.hpp"
#include <fstream>
#include <sstream>

class HttpResponse : public Http
{
	private:
		int					_statusCode;
		std::string 		_statusMessage;
		std::string 		_fileResponse;
		std::string 		_serverName;
		std::string 		_path;
		std::string 		_port;
		std::string			_method;
		ConfigFileHandle	*_config;
		std::map<int, std::string>			_status;
		std::map<int, std::string>			_fileError;
		std::map<std::string, std::string>	_header;
		// read from ConfigFileHandle
		int					_config_ser;
		std::string			_config_location;
		std::string			_config_root;
		std::vector<std::string>	_config_condition;
		// func check and set before send response
		int					_checkPort();
		int					_checkPath();
		void				_setHeader(std::string const &key, std::string const &value);
		void				_setConfigCondition();
		void				_checkMethod();
		void				_setRootPath();
		void				_setFileResponse(std::string const &pathFile, std::string const &rootPath);
		std::string			_setConfigCondition(std::string const &nameCondition);
		std::string			_searchIndex(std::string const &pathFile);
		std::string			_checkFile();
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
		std::string		returnResponse();
};

#endif