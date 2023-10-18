#include <iostream>
#include <string>
#include "color.hpp"

#include "ServHandle.hpp"

#include <csignal>

ServHandle	servManager;

// Signal handler function
void handle_sigint(int sig) {
    std::cout << "Received SIGINT (Ctrl+C). Exiting..." << std::endl;
    // You can perform cleanup or take other actions here
    // exit(0); // Terminate the program
	servManager.servStop();
}

int	main(int argc, char ** argv) {

	// ServHandle	servManager;

	// / Register the signal handler for SIGINT
	std::signal(SIGINT, handle_sigint);

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
