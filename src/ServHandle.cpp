#include "ServHandle.hpp"

ServHandle::ServHandle(void) {}

ServHandle::~ServHandle(void) {}

void	ServHandle::servCreate(char const * configFile) {
	std::cout << MAG << "servCreate" << reset << std::endl;
	this->_configServ.readConfigFile(configFile);
}

void	ServHandle::servStart(void) {}

void	ServHandle::servStop(void) {}
