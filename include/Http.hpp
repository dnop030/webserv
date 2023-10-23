#pragma once

#include <iostream>
#include <map>
#include <vector>

class Http
{
protected:
	// std::map<std::string, std::string> _start_line;
	std::string _start_line[3];
	std::map<std::string, std::string> _header;
	std::string _body;
	std::vector<std::string> _bodyChunk;

public:
	Http();
	Http(Http const &src);
	Http &operator=(const Http &src);
	std::map<std::string, std::string> getHeader();
	std::string getBody();
	std::vector<std::string> getBodyChunk();
	virtual ~Http();
};
