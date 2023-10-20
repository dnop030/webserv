#include "ConfigFileHandle.hpp"

ConfigFileHandle::ConfigFileHandle(void) {
	this->_tmpServConfigDetail = NULL;
	this->_amountServConfigDetail = 0;
	this->_tmpStr.clear();
}

ConfigFileHandle::~ConfigFileHandle(void) {
	std::map<unsigned int, ServConfigDetail *>::iterator	iter = this->_servConfig.begin();
	while (iter != this->_servConfig.end()) {
		delete	iter->second;
		iter++;
	}
}

void	ConfigFileHandle::readConfigFile(char const * fileName) {
	// std::cout << MAG << "read Server Config File" << reset << std::endl;
	std::ifstream	ifs;

	std::string		tmpRd;

	ifs.open(fileName, std::ifstream::in);
	if (ifs.good() == false) {
		ifs.close();
		std::cout << RED << fileName << " file not found or no permission. please input valid filename" << reset << std::endl;
		// throw !!!!!!!!!!!!!!!!!!!!!!1
		exit(1);
	}
	else {
		while (std::getline(ifs, tmpRd)) {
			// std::cout << tmpRd;
			// std::cout << std::endl;
			if ((tmpRd.find("{") != std::string::npos) && (tmpRd.length() == 1)) {
				this->_servConfig[this->_amountServConfigDetail] = new ServConfigDetail();

				// std::cout << YEL << "Add of ServConfigDetai " << this->_amountServConfigDetail << " ";
				// std::cout << this->_servConfig[this->_amountServConfigDetail] << reset << std::endl;

				// std::cout << "Create New ServerConfigDetail" << std::endl;
				this->_amountServConfigDetail++;
			}
			else if ((tmpRd.find("location") != std::string::npos) && (tmpRd.find("{") != std::string::npos)) {

			}
			else if ((tmpRd.length() > 1) && (tmpRd.find(";") != std::string::npos)) {
				// std::cout << tmpRd;
				// std::cout << std::endl;
				this->_tmpServConfigDetail = this->_servConfig[this->_amountServConfigDetail - 1];

				// std::cout << MAG << "Add of ServConfigDetai " << this->_amountServConfigDetail - 1 << " ";
				// std::cout << this->_tmpServConfigDetail << reset << std::endl;

				this->_tmpServConfigDetail->storeConfig(tmpRd);
			}
		}
		ifs.close();
	}
}

void	ConfigFileHandle::showDetailConfigFile(void) {
	for (int i = 0; i < this->_amountServConfigDetail; i++) {
		this->_servConfig[i]->showDetail();
		std::cout << std::endl;
	}
}

unsigned int		ConfigFileHandle::getAmountServConfig(void) {
	return (this->_amountServConfigDetail);
}

std::string const &	ConfigFileHandle::getServConfigVal(int servNum, std::string const & key) {
	std::map<unsigned int, ServConfigDetail *>::iterator	it;

	std::cout << MAG << "[INFO]configFileHandle getServConfigVal" << reset << std::endl;
	it = this->_servConfig.find(servNum);
	if (it != this->_servConfig.end()) {
		return (it->second->getVal(key));
	}
	else {
		this->_tmpStr.clear();
		return (this->_tmpStr);
	}
}
