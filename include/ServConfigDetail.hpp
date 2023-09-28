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

		void	storeConfig(std::string const & config);
		void	showDetail(void);
		std::string const &	getVal(std::string const & val);

	private:
		ServConfigDetail(ServConfigDetail const & r);
		ServConfigDetail &operator=(ServConfigDetail const & r);

		std::multimap<std::string, std::string>	_detail;
		std::string							tmpStr;
};

#endif
