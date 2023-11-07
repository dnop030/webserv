#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "Request.hpp"
#include "UtilsFunction.hpp"

class Request;

class FormData
{
private:
	std::vector<std::map<std::string, std::string>> _form;
	Request *_req;
	std::string *_split;

public:
	FormData(Request &req);
	FormData(FormData const &src);
	FormData &operator=(FormData const &src);
	~FormData();

	int ft_strlen_size_t(std::string &str);
	void getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound);

	void checkEndBound(std::string::size_type start, std::string::size_type end);
	void checkContentType();

	void parseHeader(std::map<std::string, std::string> &map, std::string &line);
	void parseToMap();
	std::map<std::string, std::string> parseChunk(std::string &chunk);

	std::vector<std::string> splitCRLF(std::string &buffer);

	std::string mapToJason(void);

	class BadRequest : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	void printForm();
};
