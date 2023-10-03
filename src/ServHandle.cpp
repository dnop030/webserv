#include "ServHandle.hpp"

ServHandle::ServHandle(void) {}

ServHandle::~ServHandle(void) {
	for (int i=0; i<this->_serv.size(); i++) {
		delete this->_serv[i];
	}
}

void	ServHandle::servCreate(char const * configFile) {
	std::cout << MAG << "servCreate" << reset << std::endl;
	this->_configServ.readConfigFile(configFile);

	// // Debug
	// this->_configServ.showDetailConfigFile();

	// std::string	tmpStr;
	// tmpStr = this->_configServ.getServConfigVal(0, "listen");
	// std::cout << MAG << "[INFO]output \"listen\" 0 " << tmpStr << reset << std::endl;

	// tmpStr = this->_configServ.getServConfigVal(0, "listen ");
	// std::cout << MAG << "[INFO]output \"listen \" 0 " << tmpStr << reset << std::endl;
	// // std::cout << MAG << "[INFO]output length " << tmpStr.length() << reset << std::endl;

	std::cout << MAG << "amout of serv config " << this->_configServ.getAmountServConfig() << reset << std::endl;
	for (int i=0; i<this->_configServ.getAmountServConfig(); i++) {
		this->_tmpServer = new Server();
		this->_tmpServer->setServSockAddr(this->_configServ.getServConfigVal(i, "listen"));
		this->_serv.push_back(this->_tmpServer);
	}
}

void	ServHandle::servStart(void) {}

void	ServHandle::servStop(void) {}
