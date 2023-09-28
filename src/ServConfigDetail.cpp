#include "ServConfigDetail.hpp"
#include <cstddef>

ServConfigDetail::ServConfigDetail(void) {}

ServConfigDetail::~ServConfigDetail(void) {}

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
	// else {
	// 	std::cout << MAG << "[INFO]NOT Found key" << reset << std::endl;
	// 	return ("test");
	// }
}
