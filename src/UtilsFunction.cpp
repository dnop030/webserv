#include "UtilsFunction.hpp"

int countChar(std::string &str, char c)
{
	int res = 0;
	for (std::string::size_type idx = 0; str[idx] != '\0'; ++idx)
	{
		if (str[idx] == c)
			++res;
	}
	return (res);
}

void removeChar(std::string &str, char c)
{
	std::string res;
	for (std::string::size_type idx = 0; str[idx] != '\0'; ++idx)
	{
		if (str[idx] != c)
			res += str[idx];
	}
	str = res;
}

void trimTail(std::string &str, char delim)
{
	std::string res;

	std::string::size_type end = str.length();
	while (end > 0 && str[end - 1] == delim)
		--end;
	for (std::string::size_type idx = 0; idx != end; ++idx)
		res += str[idx];
	str = res;
}

void trimHead(std::string &str, char delim)
{
	std::string::size_type start = skipChar(str, 0, delim);
	str = str.substr(start);
}

void trimTail_str(std::string &str, std::string delim)
{
	std::string res;

	std::string::size_type end = str.find_last_of(delim);
	while (end == std::string::npos)
	{
		end -= ft_strlen(delim);
		end = str.find_last_of(delim);
	}
	res = str.substr(0, end - 0);
	str = res;
}

void trimHeadTail(std::string &str, char delim)
{
	trimHead(str, delim);
	trimTail(str, delim);
}

std::string::size_type skipChar(std::string &str, std::string::size_type idx, char c)
{
	while (str[idx] == c && str[idx] != '\0')
		++idx;
	return (idx);
}

double ft_stod(const std::string &s)
{
	double res = 0;

	res = atof(s.c_str());
	return (res);
}

size_t ft_strlen(std::string &str)
{
	size_t i = 0;
	while (str[i] != '\0')
		++i;
	return (i);
}

int allDigit(std::string &str)
{
	for (int i = 0; str[i] != '\0'; ++i)
	{
		if (!std::isdigit(str[i]))
			return (0);
	}
	return (1);
}

int ft_strncmp(std::string &str1, std::string &str2, size_t size)
{
	for (size_t i = 0; i < size && str1[i] != '\0' && str2[i] != '\0'; ++i)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
	}
	return (0);
}

bool isHex(std::string &str)
{
	for (int i = 0; str[i] != '\0'; ++i)
	{
		if (!std::isdigit(str[i]) && ((str[i] < 'A' || str[i] > 'F') && (str[i]) < 'a' || str[i] > 'f'))
			return (false);
	}
	return (true);
}

int ft_htod(std::string &str)
{
	int res = 0;

	for (int i = 0; str[i] != '\0'; ++i)
	{
		if ((str[i] >= 'a' && str[i] <= 'f'))
			res = (res * 16) + (str[i] - 'a' + 10);
		else if ((str[i] >= 'A' && str[i] <= 'F'))
			res = (res * 16) + (str[i] - 'A' + 10);
		else if (std::isdigit(str[i]))
			res = (res * 16) + (str[i] - '0');
		else
			return (-1);
	}
	return (res);
}

std::string::size_type getLastChar(std::string &str)
{
	std::string::size_type res = 0;
	while (str[res] != '\0')
		++res;
	return (res);
}

std::string *ft_split(std::string &str, char delim)
{
	int size = 0;
	std::string *res;

	std::string::size_type start = 0;
	trimTail(str, delim);
	start = skipChar(str, start, delim);
	std::string::size_type tmp_idx = start;
	while (str[start] != '\0')
	{
		if (str[start] == delim)
		{
			++size;
			start = skipChar(str, start, delim);
		}
		else
			++start;
	}
	if (size == 0)
		return (NULL);
	res = new std::string[size + 2];

	int i = 0;
	start = 0;
	std::string::size_type end = str.find(delim, tmp_idx);
	while (end != std::string::npos && i < size)
	{
		if (str[end] == delim)
		{
			res[i++] = str.substr(start, end - start);
			start = end;
			start = skipChar(str, start, delim);
			end = str.find(delim, start);
		}
	}
	res[i++] = str.substr(start);
	res[i] = "";
	return (res);
}

std::string ft_itoa(int num)
{
	std::stringstream ss;
	ss << num;
	return (ss.str());
}

std::string currentDate(void)
{
	std::time_t currentTime = std::time(nullptr);
	struct tm *localTime = std::localtime(&currentTime);

	int year;
	std::string month;
	int day;
	std::string w_day;
	int hour;
	int minute;
	int second;

	if (localTime)
	{
		year = localTime->tm_year + 1900;
		day = localTime->tm_mday;
		localTime->tm_wday;
		hour = localTime->tm_hour + 7;
		minute = localTime->tm_min;
		second = localTime->tm_sec;

		if (localTime->tm_mon + 1 == 1)
			month = "Jan";
		else if (localTime->tm_mon + 1 == 2)
			month = "Feb";
		else if (localTime->tm_mon + 1 == 3)
			month = "Mar";
		else if (localTime->tm_mon + 1 == 4)
			month = "Apr";
		else if (localTime->tm_mon + 1 == 5)
			month = "May";
		else if (localTime->tm_mon + 1 == 6)
			month = "Jun";
		else if (localTime->tm_mon + 1 == 7)
			month = "Jul";
		else if (localTime->tm_mon + 1 == 8)
			month = "Aug";
		else if (localTime->tm_mon + 1 == 9)
			month = "Sep";
		else if (localTime->tm_mon + 1 == 10)
			month = "Oct";
		else if (localTime->tm_mon + 1 == 11)
			month = "Nov";
		else if (localTime->tm_mon + 1 == 12)
			month = "Dec";

		if (localTime->tm_wday == 0)
			w_day = "Sun";
		else if (localTime->tm_wday == 1)
			w_day = "Mon";
		else if (localTime->tm_wday == 2)
			w_day = "Tue";
		else if (localTime->tm_wday == 3)
			w_day = "Wed";
		else if (localTime->tm_wday == 4)
			w_day = "Thu";
		else if (localTime->tm_wday == 5)
			w_day = "Fri";
		else if (localTime->tm_wday == 6)
			w_day = "Sat";
	}
	std::string res_hour;
	std::string res_minute;
	std::string res_second;
	if (hour < 10)
		res_hour = "0" + ft_itoa(hour);
	else
		res_hour = ft_itoa(hour);
	if (minute < 10)
		res_minute = "0" + ft_itoa(minute);
	else
		res_minute = ft_itoa(minute);
	if (second < 10)
		res_second = "0" + ft_itoa(second);
	else
		res_second = ft_itoa(second);
	return (w_day + ", " + ft_itoa(day) + " " + month + " " + ft_itoa(year) + " " + res_hour + ":" + res_minute + ":" + res_second + " GMT");
}
