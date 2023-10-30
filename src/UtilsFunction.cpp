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
