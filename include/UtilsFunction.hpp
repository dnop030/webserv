#pragma once

#include <iostream>

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
