#pragma once

#include <iostream>
#include <map>

class Http
{
protected:
	// std::map<std::string, std::string> _start_line;
	std::string _start_line[3];
	std::map<std::string, std::string> _header;
	std::string _body;

public:
	Http();
	Http(Http const &src);
	Http &operator=(const Http &src);
	std::map<std::string, std::string> getHeader();
	std::string getBody();
	virtual ~Http();
};
