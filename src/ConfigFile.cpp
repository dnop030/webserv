#include "ConfigFile.hpp"

ConfigFile::ConfigFile(void) {}

ConfigFile::~ConfigFile(void) {}

void	ConfigFile::readConfigFile(char const * fileName) {
	// std::cout << MAG << "read Server Config File" << reset << std::endl;
	std::ifstream	ifs;

	std::string		tmpRd;

	ifs.open(fileName, std::ifstream::in);
	if (ifs.good() == false) {
		ifs.close();
		std::cout << RED << "file not found or no permission. please input valid filename" << reset << std::endl;
		// throw !!!!!!!!!!!!!!!!!!!!!!1
		exit(1);
	}
	else {
		while (std::getline(ifs, tmpRd)) {
			std::cout << tmpRd;
			std::cout << std::endl;
		}
		ifs.close();
	}
}
