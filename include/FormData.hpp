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
	std::string *_split;

public:
	FormData(Request &req);
	FormData(FormData const &src);
	FormData &operator=(FormData const &src);
	~FormData();

	int ft_strlen_size_t(std::string &str);
	void trimTail_str(std::string &str, std::string delim);
	void trimHead(std::string &str, char delim);
	void trimHeadTail(std::string &str, char delim);
	void getFromBound(std::vector<std::string> &chunk, std::string::size_type &start, std::string &bound);

	void checkEndBound(std::string::size_type start, std::string::size_type end);
	void checkContentType();

	void parseHeader(std::map<std::string, std::string> &map, std::string &line);
	void parseToMap();
	std::map<std::string, std::string> parseChunk(std::string &chunk);

	std::vector<std::string> splitCRLF(std::string &buffer);

	class BadRequest : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	void printForm();
};
