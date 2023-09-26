#include "ServHandle.hpp"

ServHandle::ServHandle(void) {}

ServHandle::~ServHandle(void) {}

void	ServHandle::servCreate(char const * configFile) {
	std::cout << YEL << "Serv Create" << reset << std::endl;
	this->_configFileServer.readConfigFile(configFile);
}

void	ServHandle::servStart(void) {}

void	ServHandle::servStop(void) {}
