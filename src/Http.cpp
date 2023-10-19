#include "Http.hpp"

Http::Http() : _body("")
{
}

Http::Http(Http const &src) : _body(src._body)
{
	*this = src;
}

Http &Http::operator=(Http const &src)
{
	if (this != &src)
	{
		for (int i = 0; i < 3; ++i)
			this->_start_line[i] = src._start_line[i];

		if (!this->_header.empty())
			this->_header.clear();
		std::map<std::string, std::string>::const_iterator it2 = src._header.begin();
		while (it2 != src._header.end())
		{
			this->_header[it2->first] = it2->second;
			++it2;
		}
	}
	return (*this);
}

Http::~Http()
{
}

std::map<std::string, std::string> Http::getHeader()
{
	std::map<std::string, std::string> res;
	if (this->_header.empty())
		return (res);
	for (std::map<std::string, std::string>::iterator it = this->_header.begin(); it != this->_header.end(); ++it)
	{
		if (res.find(it->first) == res.end())
			res[it->first] = it->second;
	}
	return (res);
}

std::string Http::getBody()
{
	return (this->_body);
}
