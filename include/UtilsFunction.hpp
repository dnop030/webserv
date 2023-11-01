#pragma once

#include <iostream>
#include "Request.hpp"
#include <ctime>
#include <sstream>

int countChar(std::string &str, char c);
void removeChar(std::string &str, char c);
void trimTail(std::string &str, char delim);
void trimHead(std::string &str, char delim);
void trimTail_str(std::string &str, std::string delim);
void trimHeadTail(std::string &str, char delim);
std::string::size_type skipChar(std::string &str, std::string::size_type idx, char c);
double ft_stod(const std::string &s);
size_t ft_strlen(std::string &str);
int allDigit(std::string &str);
int ft_strncmp(std::string &str1, std::string &str2, size_t size);
bool isHex(std::string &str);
int ft_htod(std::string &str);
std::string::size_type getLastChar(std::string &str);
std::string *ft_split(std::string &str, char delim);
std::string currentDate(void);
std::string ft_atoi(int num);

template <typename Map, typename Key, typename Value>
void addKey(Map &map, Key &key, Value &value)
{
	if (map.find(key) != map.end())
		return;
	map[key] = value;
}

template <typename Key, typename Value>
void deepCopyMap(std::map<Key, Value> &src, std::map<Key, Value> &target)
{
	if (!target.empty())
		target.clear();
	for (typename std::map<Key, Value>::iterator it = src.begin(); it != src.end(); ++it)
		addKey(target, it->first, it->second);
}

template <typename Key, typename Value>
void printKeyValue(std::map<Key, Value> &map, Key key)
{
	typename std::map<Key, Value>::const_iterator it = map.find(key);
	if (it != map.end())
		std::cout << it->first << ": " << it->second << "\r\n";
	// std::cout << it->first << ": " << it->second << std::endl;
}
