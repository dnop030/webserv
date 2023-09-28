#include "ServHandle.hpp"

ServHandle::ServHandle(void) {}

ServHandle::~ServHandle(void) {}

void	ServHandle::servCreate(char const * configFile) {
	std::cout << MAG << "servCreate" << reset << std::endl;
	this->_configServ.readConfigFile(configFile);

	this->_configServ.showDetailConfigFile();

	std::string	tmpStr;
	tmpStr = this->_configServ.getServConfigVal(0, "listen");
	std::cout << MAG << "[INFO]output \"listen\" 0 " << tmpStr << reset << std::endl;

	tmpStr = this->_configServ.getServConfigVal(0, "listen ");
	std::cout << MAG << "[INFO]output \"listen \" 0 " << tmpStr << reset << std::endl;
	// std::cout << MAG << "[INFO]output length " << tmpStr.length() << reset << std::endl;
}

void	ServHandle::servStart(void) {}

void	ServHandle::servStop(void) {}
