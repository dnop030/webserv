#include <iostream>
#include <string>
#include "color.hpp"

#include "ServHandle.hpp"

int	main(int argc, char ** argv) {

	ServHandle	servManager;
	if (argc <= 2) {
		if (argc == 2) {
			std::cout << GRN << "Config file name " << argv[1] << reset << std::endl;
			servManager.servCreate(argv[1]);
		}
		else {
			std::cout << YEL << "Config file name Default config file" << reset << std::endl;
			servManager.servCreate("config/webserv.conf");
		}
		servManager.servStart();
	}
	else {
		std::cout << RED << "Example cmd : ./webserv [configuration file]\n" << reset << std::endl;
	}
	return (0);
}
