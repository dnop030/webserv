#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "color.hpp"

class ConfigFile
{
	public:
		ConfigFile(void);
		~ConfigFile(void);

		void	readConfigFile(char const * fileName);

	private:
		ConfigFile(ConfigFile const & r);
		ConfigFile &operator=(ConfigFile const & r);
};

#endif
