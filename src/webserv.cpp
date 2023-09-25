#include <iostream>
#include <string>
#include "color.hpp"

int	main(int argc, char ** argv) {
	if (argc <= 2) {
		if (argc == 2)
			std::cout << GRN << "Config file name " << argv[1] << reset << std::endl;
		else
			std::cout << YEL << "Config file name Default config file" << reset << std::endl;
	}
	else {
		std::cout << RED << "Example cmd : ./webserv [configuration file]\n" << reset << std::endl;
	}
	return (0);
}
