#include "ConfigFileHandle.hpp"

ConfigFileHandle::ConfigFileHandle(void) {
	this->_tmpServConfigDetail = NULL;
	this->_amountServConfigDetail = 0;
	this->_tmpStr.clear();
	// this->_foundLocation = 0;
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
		int			cntOpenBlacket = 0;
		int			cntCloseBlacket = 0;
		int			sndBlockConfig = 0;
		int			foundServKeyW = 0;
		std::string	tmpOneBlockServConfig;

		tmpOneBlockServConfig.clear();

		while (std::getline(ifs, tmpRd)) {

			// new code
			if (tmpRd.find("server ") != std::string::npos) {
				foundServKeyW = 1;
			}
			if (tmpRd.find("{") != std::string::npos) {
				cntOpenBlacket++;
			}
			if (tmpRd.find("}") != std::string::npos) {
				cntCloseBlacket++;
			}
			if (foundServKeyW == 1) {
				tmpOneBlockServConfig += tmpRd += "\n";
			}

			// if (cntCloseBlacket == cntOpenBlacket) {
			// if ((cntCloseBlacket == cntOpenBlacket) && (foundServKeyW == 1)) {
			if ((cntCloseBlacket == cntOpenBlacket) && (foundServKeyW == 1) && (cntOpenBlacket > 0)) {
				// debug
				// std::cout << tmpOneBlockServConfig << std::endl;
				// std::cout << "last size " << this->_amountServConfigDetail << " from " << this->_servConfig.size() << std::endl;

				this->_servConfig[this->_servConfig.size()] = new ServConfigDetail();
				// this->_servConfig[this->_servConfig.size()].saveOndBlockConfig(tmpOneBlockServConfig);
				this->_tmpServConfigDetail = this->_servConfig[this->_servConfig.size() - 1];
				this->_tmpServConfigDetail->saveOneBlockConfig(tmpOneBlockServConfig);

				cntOpenBlacket = 0;
				cntCloseBlacket = 0;
				sndBlockConfig = 0;
				foundServKeyW = 0;
				tmpOneBlockServConfig.clear();
				this->_amountServConfigDetail++;
			}

			// // debug
			// std::cout << MAG << "open " << cntOpenBlacket << " close " << cntCloseBlacket << reset << std::endl;
			// std::cout << MAG << tmpOneBlockServConfig << reset << std::endl;


			// original code
			// // std::cout << tmpRd;
			// // std::cout << std::endl;

			// // malloc when found new config server
			// // if ((tmpRd.find("{") != std::string::npos) && (tmpRd.length() == 1)) {
			// if ((tmpRd.find("{") != std::string::npos) && (tmpRd.length() == 1) && (this->_foundLocation == 0)) {
			// 	this->_servConfig[this->_amountServConfigDetail] = new ServConfigDetail();

			// 	// std::cout << YEL << "Add of ServConfigDetai " << this->_amountServConfigDetail << " ";
			// 	// std::cout << this->_servConfig[this->_amountServConfigDetail] << reset << std::endl;

			// 	// std::cout << "Create New ServerConfigDetail" << std::endl;
			// 	this->_amountServConfigDetail++;
			// }
			// // found new location in server config
			// // else if ((tmpRd.find("location") != std::string::npos) && (tmpRd.find("{") != std::string::npos)) {
			// else if ((tmpRd.find("location") != std::string::npos) && (tmpRd.find("{") != std::string::npos) && (this->_foundLocation == 0)) {
			// 	// std::cout << MAG << "[INFO] found start location " << tmpRd << reset << std::endl;
			// 	this->_tmpServConfigDetail->storeConfigLocation(tmpRd);
			// 	this->_foundLocation = 1;
			// }
			// // end one group location in server config
			// else if ((this->_foundLocation == 1) && (tmpRd.find("}") != std::string::npos)) {
			// 	std::cout << MAG << "[INFO] found end location " << tmpRd << reset << std::endl;
			// 	this->_foundLocation = 0;
			// }
			// // detail of server config
			// // else if ((tmpRd.length() > 1) && (tmpRd.find(";") != std::string::npos)) {
			// else if ((tmpRd.length() > 1) && (tmpRd.find(";") != std::string::npos) && (this->_foundLocation == 0)) {
			// 	// std::cout << tmpRd;
			// 	// std::cout << std::endl;
			// 	this->_tmpServConfigDetail = this->_servConfig[this->_amountServConfigDetail - 1];

			// 	// std::cout << MAG << "Add of ServConfigDetai " << this->_amountServConfigDetail - 1 << " ";
			// 	// std::cout << this->_tmpServConfigDetail << reset << std::endl;

			// 	this->_tmpServConfigDetail->storeConfig(tmpRd);
			// }
			// // detail of location in server config
			// else if ((tmpRd.length() > 1) && (tmpRd.find(";") != std::string::npos) && (this->_foundLocation == 1)) {
			// 	// std::cout << MAG << "[INFO] Detail location " << tmpRd << reset << std::endl;
			// 	this->_tmpServConfigDetail->storeConfigLocation(tmpRd);
			// }
		}
		// if ((cntCloseBlacket == cntOpenBlacket) && (foundServKeyW == 1) && (cntOpenBlacket > 0)) {
		// 	// // dugb
		// 	// std::cout << tmpOneBlockServConfig << std::endl;
		// 	// std::cout << "size " << this->_amountServConfigDetail << std::endl;

		// 	this->_servConfig[this->_servConfig.size()] = new ServConfigDetail();
		// 	// this->_servConfig[this->_servConfig.size()].saveOndBlockConfig(tmpOneBlockServConfig);
		// 	this->_tmpServConfigDetail = this->_servConfig[this->_servConfig.size() - 1];
		// 	this->_tmpServConfigDetail->saveOneBlockConfig(tmpOneBlockServConfig);

		// 	cntOpenBlacket = 0;
		// 	cntCloseBlacket = 0;
		// 	sndBlockConfig = 0;
		// 	foundServKeyW = 0;
		// 	tmpOneBlockServConfig.clear();
		// 	this->_amountServConfigDetail++;
		// }
		// std::cout << "last size " << this->_amountServConfigDetail << " from " << this->_servConfig.size() << std::endl;
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
