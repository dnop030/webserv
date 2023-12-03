#include "Http.hpp"

Http::Http() : _body(""), _filename("")
{
}

Http::Http(Http const &src) : _body(src._body), _filename(src._filename)
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

		if (!this->_respHeader.empty())
			this->_respHeader.clear();
		it2 = src._respHeader.begin();
		while (it2 != src._respHeader.end())
		{
			this->_respHeader[it2->first] = it2->second;
			++it2;
		}

		if (!this->_bodyChunk.empty())
			this->_bodyChunk.clear();
		for (std::vector<std::string>::const_iterator it = src._bodyChunk.begin(); it != src._bodyChunk.end(); ++it)
			this->_bodyChunk.push_back(*it);
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

std::string Http::getFilename()
{
	return (this->_filename);
}

std::vector<std::string> Http::getBodyChunk()
{
	std::vector<std::string> res;
	for (std::vector<std::string>::iterator it = this->_bodyChunk.begin(); it != this->_bodyChunk.end(); ++it)
		res.push_back(*it);
	return (res);
}
