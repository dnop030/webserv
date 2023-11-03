#ifndef SERVCONFIGDETAIL_HPP
#define SERVCONFIGDETAIL_HPP

#include <string>
#include <iostream>
#include <cctype>
#include "color.hpp"

#include <map>

class ServConfigDetail
{
	public:
		ServConfigDetail(void);
		~ServConfigDetail(void);

		void	saveOneBlockConfig(std::string const & block);
		void	storeConfig(std::string const & config);
		void	showDetail(void);
		std::string const &	getVal(std::string const & val);

		// void	storeConfigLocation(std::string const & location);
		// int		getAmountLocation(void);
		// std::string const &	getValLocation(int index, std::string const & key);

	private:
		ServConfigDetail(ServConfigDetail const & r);
		ServConfigDetail &operator=(ServConfigDetail const & r);

		std::multimap<std::string, std::string>	_detail;
		std::string							_tmpStr;
		// int									_amountLocation;

		// std::multimap<int, LocationConfigDetail *>	_locationDetail;
		// std::multimap<std::string, std::string>	_locationDetail;
};

#endif
