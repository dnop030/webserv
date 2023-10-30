#include "CheckHeader.hpp"

CheckHeader::CheckHeader(Request &req) : _contentLang("*")
{
	try
	{
		this->_req = new Request(req);
		this->checkContentLength();
		this->checkDate();
		this->contentNego();
	}
	catch (const LengthRequired &e)
	{
		this->_req->_statusCode = 411;
		std::cout << e.what() << std::endl;
	}
	catch (const BadRequest &e)
	{
		this->_req->_statusCode = 400;
		std::cout << e.what() << std::endl;
	}
}

CheckHeader::CheckHeader(CheckHeader const &src)
{
	*this = src;
}

CheckHeader &CheckHeader::operator=(CheckHeader const &src)
{
	if (this != &src)
	{
		if (this->_req != NULL)
			delete this->_req;
		this->_req = new Request(*src._req);
		this->_t.tm_year = src._t.tm_year;
		this->_t.tm_mon = src._t.tm_mon;
		this->_t.tm_mday = src._t.tm_mday;
		this->_t.tm_hour = src._t.tm_hour;
		this->_t.tm_min = src._t.tm_min;
		this->_t.tm_sec = src._t.tm_sec;
		this->_t.tm_isdst = src._t.tm_isdst;
		this->_contentLang = src._contentLang;
	}
	return (*this);
}

CheckHeader::~CheckHeader()
{
	if (this->_req != NULL)
	{
		delete this->_req;
		this->_req = NULL;
	}
}

const char *CheckHeader::BadRequest::what() const throw()
{
	return ("400 Bad request");
}

const char *CheckHeader::LengthRequired::what() const throw()
{
	return ("411 Length required");
}

void CheckHeader::ft_error(std::string *split)
{
	if (split != NULL)
		delete[] split;
	throw BadRequest();
}

void CheckHeader::checkDate(void)
{
	if (this->_req->_header.find("Date") == this->_req->_header.end())
		return;
	std::string tmp = this->_req->_header["Date"];
	std::string *date = this->_req->ft_split(tmp, ' ');
	int i = 0;
	while (date != NULL && !date[i].empty())
		++i;
	if (date == NULL || i != 6)
		this->ft_error(date);
	if (date[0] != "Mon," && date[0] != "Tue," && date[0] != "Wed," && date[0] != "Thu," && date[0] != "Fri," && date[0] != "Sat," && date[0] != "Sun,")
		this->ft_error(date);
	if (date[1].size() != 2 || !allDigit(date[1]))
		this->ft_error(date);
	int day = static_cast<int>(ft_stod(date[1]));
	int month;
	if (date[2] == "Jan")
		month = 1;
	else if (date[2] == "Feb")
		month = 2;
	else if (date[2] == "Mar")
		month = 3;
	else if (date[2] == "Apr")
		month = 4;
	else if (date[2] == "May")
		month = 5;
	else if (date[2] == "Jun")
		month = 6;
	else if (date[2] == "Jul")
		month = 7;
	else if (date[2] == "Aug")
		month = 8;
	else if (date[2] == "Sep")
		month = 9;
	else if (date[2] == "Oct")
		month = 10;
	else if (date[2] == "Nov")
		month = 11;
	else if (date[2] == "Dec")
		month = 12;
	else
		this->ft_error(date);
	if (date[3].size() != 4 || !allDigit(date[3]))
		this->ft_error(date);
	int year = static_cast<int>(ft_stod(date[3]));
	if (date[5] != "GMT")
		this->ft_error(date);
	std::string *clock = this->_req->ft_split(date[4], ':');
	delete[] date;
	i = 0;
	while (clock != NULL && !clock[i].empty())
		++i;
	if (clock == NULL || i != 3)
		throw BadRequest();
	if (clock[0].size() != 2 || !allDigit(clock[0]))
		this->ft_error(clock);
	int hour = static_cast<int>(ft_stod(clock[0]));
	if (clock[1].size() != 2 || !allDigit(clock[1]))
		this->ft_error(clock);
	int minute = static_cast<int>(ft_stod(clock[1]));
	if (clock[2].size() != 2 || !allDigit(clock[2]))
		this->ft_error(clock);
	int second = static_cast<int>(ft_stod(clock[2]));
	delete[] clock;
	int y_tmp = year - 1900;
	int m_tmp = month - 1;
	int d_tmp = day;
	int h_tmp = hour;
	int min_tmp = minute;
	int sec_tmp = second;
	this->_t.tm_year = y_tmp;
	this->_t.tm_mon = m_tmp;
	this->_t.tm_mday = d_tmp;
	this->_t.tm_hour = h_tmp;
	this->_t.tm_min = min_tmp;
	this->_t.tm_sec = sec_tmp;
	this->_t.tm_isdst = 0;
	time_t time = mktime(&this->_t);
	if (this->_t.tm_year != y_tmp || this->_t.tm_mon != m_tmp || this->_t.tm_mday != d_tmp || this->_t.tm_hour != h_tmp || this->_t.tm_min != min_tmp || this->_t.tm_sec != sec_tmp)
		throw BadRequest();
}

void CheckHeader::checkContentLength()
{
	if (!this->_req->_body.empty() && this->_req->_header.find("Content-Length") == this->_req->_header.end() && this->_req->_header.find("Transfer-Encoding") == this->_req->_header.end())
		throw LengthRequired();
	if (!this->_req->_body.empty() && this->_req->_header.find("Content-Length") != this->_req->_header.end() && this->_req->_header.find("Transfer-Encoding") != this->_req->_header.end())
		this->_req->_header.erase("Content-Length");
}

float CheckHeader::checkAcceptLanguage(std::string &str, float q)
{
	float tmp_q = q;
	std::string *tmp = this->_req->ft_split(str, ';');
	if (tmp == NULL)
	{
		this->_contentLang = str;
		return (1);
	}
	if (tmp[1][0] != 'q' && tmp[1][1] != '=')
	{
		delete[] tmp;
		return (-1);
	}
	std::string *tmp2 = this->_req->ft_split(tmp[1], '=');
	std::string lang = tmp[0];
	delete[] tmp;
	std::string weight = tmp2[1];
	delete[] tmp2;
	if (weight[0] < '0' || weight[0] > '1')
		return (-1);
	if ((weight.size() > 1 && weight[1] != '.') || weight.size() == 2)
		return (-1);
	for (int i = 2; weight[i] != '\0'; ++i)
	{
		if (weight[i] < '0' || weight[i] > '9')
			return (-1);
	}
	q = static_cast<float>(ft_stod(weight));
	if (q > tmp_q)
	{
		this->_contentLang = lang;
		return (q);
	}
	return (tmp_q);
}

void CheckHeader::contentNego(void)
{
	if (this->_req->_header.find("Accept-Language") == this->_req->_header.end())
		return;
	std::string *tmp = this->_req->ft_split(this->_req->_header["Accept-Language"], ',');
	if (tmp != NULL)
	{
		std::vector<std::string> token;
		float q = -1;
		for (int i = 0; !tmp[i].empty(); ++i)
		{
			q = checkAcceptLanguage(tmp[i], q);
			if (q == 1 || q == -1)
				break;
		}
		delete[] tmp;
		if (q == -1)
			throw BadRequest();
	}
	else
		this->_contentLang = this->_req->_header["Accept-Language"];
	trimHead(this->_contentLang, ' ');
	std::cout << "@@@@@@ content language is @@@@@@" << std::endl;
	std::cout << this->_contentLang << std::endl;
}
