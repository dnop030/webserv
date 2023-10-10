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
		// if (!this->_start_line.empty())
		// 	this->_start_line.clear();
		// std::map<std::string, std::string>::const_iterator it = src._start_line.begin();
		// while (it != src._start_line.end())
		// {
		// 	this->_start_line[it->first] = it->second;
		// 	++it;
		// }
		for (int i = 0; i < 3; ++i)
			this->_start_line[i] = src._start_line[i];

		if (this->_header.empty())
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
