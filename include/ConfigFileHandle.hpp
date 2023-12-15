#ifndef CONFIGFILEHANDLE_HPP
#define CONFIGFILEHANDLE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "color.hpp"
#include <map>
#include "ServConfigDetail.hpp"

class ConfigFileHandle
{
	public:
		ConfigFileHandle(void);
		~ConfigFileHandle(void);

		int		readConfigFile(char const * fileName);
		void	showDetailConfigFile(void);
		unsigned int		getAmountServConfig(void);
		std::string const &	getServConfigVal(int servNum, std::string const & key);

	private:
		ConfigFileHandle(ConfigFileHandle const & r);
		ConfigFileHandle &operator=(ConfigFileHandle const & r);

		std::map<unsigned int, ServConfigDetail *>	_servConfig;
		unsigned int					_amountServConfigDetail;
		ServConfigDetail				*_tmpServConfigDetail;
		std::string						_tmpStr;
		// int								_foundLocation;
};

#endif
