#include "ServConfigDetail.hpp"

ServConfigDetail::ServConfigDetail(void) {
	this->_amountLocation = 0;
}

ServConfigDetail::~ServConfigDetail(void) {
	std::multimap<int, LocationConfigDetail *>::iterator	it = this->_locationDetail.begin();
	while (it != this->_locationDetail.end()) {
		delete	it->second;
		it++;
	}
}

void	ServConfigDetail::storeConfig(std::string const & config) {
	size_t		pos_space;
	size_t		pos_semicolon;
	std::string	key;
	std::string	val;

	pos_space = config.find(" ");
	pos_semicolon = config.find(";");

	if ((pos_space == std::string::npos) || (pos_semicolon == std::string::npos)) {
		std::cout << RED << "[ERROR]Format of config file error" << reset << std::endl;
		if (pos_space == std::string::npos)
			std::cout << BLU << "[ERROR]Not found space" << reset << std::endl;
		if (pos_semicolon == std::string::npos)
			std::cout << BLU << "[ERROR]Not found semicolon" << reset << std::endl;
		return;
	}
	else {
		// std::cout << RED << config << reset << std::endl;
		// std::cout << GRN << "Key @" << pos_space << " " << "val @" << pos_semicolon << reset << std::endl;

		key = config.substr(0, pos_space - 0);
		while (isblank(key[0])) {
			key.erase(0, 1);
		}
		val = config.substr(pos_space + 1, (pos_semicolon - pos_space) - 1);

		// std::cout << "key " << key << std::endl;
		// std::cout << GRN << "val " << val << reset << std::endl;

		this->_detail.insert(std::pair<std::string, std::string>(key, val));
	}
}

void	ServConfigDetail::showDetail(void) {
	std::multimap<std::string, std::string>::iterator	it = this->_detail.begin();
	while (it != this->_detail.end()) {
		std::cout << MAG << it->first << ":" << it->second << reset << std::endl;
		it++;
	}
}

std::string const &	ServConfigDetail::getVal(std::string const & val) {
	std::multimap<std::string, std::string>::iterator	it;

	std::cout << MAG << "[INFO]ServConfigDetail getVal" << reset << std::endl;
	it = this->_detail.find(val);
	if (it != this->_detail.end()) {
		std::cout << MAG << "[INFO]Found key" << reset << std::endl;
		return (it->second);
	}
	else {
		this->_tmpStr.clear();
		std::cout << MAG << "[INFO]NOT Found key:" << this->_tmpStr << reset << std::endl;
		return (this->_tmpStr);
	}
}

void	ServConfigDetail::storeConfigLocation(std::string const & location) {
	if (location.find("{") != std::string::npos) {
		std::cout << "Found open new location" << std::endl;
		std::cout << MAG << "detail " << location << reset << std::endl;

		// LocationConfigDetail	*tmpLocation = new LocationConfigDetail();
		// this->_locationDetail[this->_amountLocation] = new LocationConfigDetail();
		std::cout << "Size of location bef add " << this->getAmountLocation() << std::endl;
		// this->_locationDetail.insert(this->_amountLocation, new LocationConfigDetail);
		this->_locationDetail.insert(std::pair<int, LocationConfigDetail *>(this->_amountLocation, new LocationConfigDetail()));
		std::cout << "Size of location aft add " << this->getAmountLocation() << std::endl;
	}
	else if (location.find("}") != std::string::npos) {
		std::cout << "Found close new location" << std::endl;
	}
	else {
		std::cout << MAG << "detail " << location << reset << std::endl;
	}
}

int		ServConfigDetail::getAmountLocation(void) {
	return (this->_locationDetail.size());
}

std::string const &	ServConfigDetail::getValLocation(int index, std::string const & key) {
}
