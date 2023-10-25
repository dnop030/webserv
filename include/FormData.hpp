#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "Request.hpp"

class Request;

class FormData
{
private:
	std::vector<std::map<std::string, std::string>> _form;
	Request *_req;

public:
	FormData(Request &req);
	FormData(FormData const &src);
	FormData &operator=(FormData const &src);
	~FormData();

	int ft_strlen_size_t(std::string &str);
	void trimTail_str(std::string &str, std::string delim);
	void trimHead(std::string &str, char delim);
	void getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound);
	void checkContentType();
	void parseToMap();

	std::vector<std::string> splitCRLF(std::string &buffer);

	class BadRequest : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};
