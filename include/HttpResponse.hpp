#ifndef HTTPREPONSE_HPP
#define HTTPREPONSE_HPP

#include "Http.hpp"
#include <fstream>
#include <sstream>

class HttpResponse : public Http
{
	private:
		int			_statusCode;
		std::string _statusMessage;
		std::string _fileResponse;
		std::string	checkFile();
		std::map<std::string, std::string> _header;

	// protected:
	public:
		HttpResponse();
		~HttpResponse();
		HttpResponse(HttpResponse const &rhs);
		HttpResponse	&operator=(HttpResponse const &rhs);

		void			setStatusCode(int statusCode);
		void			setStatusMessage(std::string const &statusMessage);
		void			setFileResponse(std::string const &fileResponse);
		void			setHeader(std::string const &key, std::string const &value);
		std::string		returnResponse();
};

#endif