#ifndef CONFIGFILEHANDLE_HPP
#define CONFIGFILEHANDLE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "color.hpp"

class ConfigFileHandle
{
	public:
		ConfigFileHandle(void);
		~ConfigFileHandle(void);

		void	readConfigFile(char const * fileName);

	private:
		ConfigFileHandle(ConfigFileHandle const & r);
		ConfigFileHandle &operator=(ConfigFileHandle const & r);
};

#endif
