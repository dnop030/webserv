#pragma once

#include "Request.hpp"
#include "UtilsFunction.hpp"
#include <ctime>

class Request;

class CheckHeader
{
private:
	Request *_req;
	struct tm _t;
	std::string _contentLang;

public:
	CheckHeader(Request &req);
	CheckHeader(CheckHeader const &src);
	CheckHeader &operator=(CheckHeader const &src);
	~CheckHeader();

	void checkContentLength(void);
	void checkDate(void);
	float checkAcceptLanguage(std::string &str, float q);
	void contentNego(void);
	void ft_error(std::string *split);

	class BadRequest : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class LengthRequired : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};
