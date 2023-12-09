#include "ServConfigDetail.hpp"

void printMap(std::multimap<std::string, std::string> &map)
{
	std::cout << RED << "//////// print multimap /////////" << reset << std::endl;
	std::multimap<std::string, std::string>::iterator it = map.begin();
	while (it != map.end())
	{
		if (it->first == "location_back /cgi_bins/.py")
			std::cout << YEL << "key: " << it->first << reset << std::endl;
		else
			std::cout << MAG << "key: " << it->first << reset << std::endl;
		std::cout << MAG << "value: " << it->second << reset << std::endl;
		++it;
	}
	std::cout << RED << "///////////////////////////////" << reset << std::endl;
}

ServConfigDetail::ServConfigDetail(void)
{
	// this->_amountLocation = 0;
}

ServConfigDetail::~ServConfigDetail(void)
{
	// std::multimap<int, LocationConfigDetail *>::iterator	it = this->_locationDetail.begin();
	// while (it != this->_locationDetail.end()) {
	// 	delete	it->second;
	// 	it++;
	// }
}

void ServConfigDetail::saveOneBlockConfig(std::string const &block)
{
	std::string tmpBlock = block;
	std::string tmpOneLine;

	// std::cout << YEL << block << reset;

	this->delAllComment(&tmpBlock);
	// std::cout << MAG << tmpBlock << reset;

	// del server {}
	// del charactor until 1st { (del including 1st {)
	tmpBlock.erase(0, tmpBlock.find("{") + 1);

	// del } until the end of file
	tmpBlock.erase(tmpBlock.find_last_of("}"), tmpBlock.find_last_of("}") - tmpBlock.size());
	// std::cout << MAG << tmpBlock << reset;

	// using loop by detecting ;
	while (tmpBlock.find(";") != std::string::npos)
	{
		this->delFrontSpace(&tmpBlock);
		// std::cout << BLU << tmpBlock << reset;

		tmpOneLine.clear();
		tmpOneLine = tmpBlock.substr(0, tmpBlock.find(";"));
		// std::cout << MAG << tmpOneLine << reset << std::endl;

		if (tmpOneLine.find("location") != std::string::npos)
		{
			// std::cout << "location" << std::endl;

			// need to re substr for location
			// due to location might consist of multiple line
			tmpOneLine.clear();
			this->storeLocationConfig(tmpBlock);
			printMap(this->_detail);
			// std::cout << MAG << tmpOneLine << reset << std::endl;

			// delete after store
			// + 1 due to include what we want to delete
			tmpBlock.erase(0, tmpBlock.find("}") + 1);
			// tmpBlock.erase(0, tmpBlock.find("}"));
		}
		else
		{
			// std::cout << "Not location" << std::endl;

			// store to multimap
			tmpOneLine.clear();
			this->storeOneLineNormalConfig(tmpBlock);
			// std::cout << MAG << tmpOneLine << reset << std::endl;

			// delete after store
			// + 1 due to include what we want to delete
			tmpBlock.erase(0, tmpBlock.find(";") + 1);
			// tmpBlock.erase(0, tmpBlock.find(";"));
		}
		// std::cout << MAG << tmpBlock << reset;
	}

	// // cut server header
	// tmpBlock.erase(0, tmpBlock.find("{") + 2);
	// // std::cout << MAG << tmpBlock << reset << std::endl;

	// tmpBlock.erase(tmpBlock.find("}"), 2);
	// // std::cout << MAG << tmpBlock << reset << "------------------" << std::endl;

	// while (tmpBlock.size() != 0) {
	// 	// delete space at front
	// 	while (isblank(tmpBlock[0])) {
	// 		tmpBlock.erase(0, 1);
	// 	}
	// 	tmpOneLine = tmpBlock.substr(0, tmpBlock.find("\n"));
	// 	tmpBlock.erase(0, tmpBlock.find("\n") + 1);
	// 	// std::cout << tmpOneLine << std::endl;
	// 	// std::cout << MAG << tmpBlock << reset << "------------------" << std::endl;

	// 	if (tmpOneLine.find("location") == std::string::npos) {
	// 		size_t		pos_space;
	// 		size_t		pos_semicolon;
	// 		std::string	key;
	// 		std::string	val;

	// 		pos_space = tmpOneLine.find(" ");
	// 		pos_semicolon = tmpOneLine.find(";");

	// 		if ((pos_space == std::string::npos) || (pos_semicolon == std::string::npos)) {
	// 			std::cout << RED << "[ERROR]Format of config file error" << reset << std::endl;
	// 			if (pos_space == std::string::npos)
	// 				std::cout << BLU << "[ERROR]Not found space" << reset << std::endl;
	// 			if (pos_semicolon == std::string::npos)
	// 				std::cout << BLU << "[ERROR]Not found semicolon" << reset << std::endl;
	// 			return;
	// 		}
	// 		else {
	// 			key = tmpOneLine.substr(0, pos_space - 0);
	// 			// while (isblank(key[0])) {
	// 			// 	key.erase(0, 1);
	// 			// }
	// 			val = tmpOneLine.substr(pos_space + 1, (pos_semicolon - pos_space) - 1);

	// 			// std::cout << "key " << key << std::endl;
	// 			// std::cout << GRN << "val " << val << reset << std::endl;

	// 			this->_detail.insert(std::pair<std::string, std::string>(key, val));
	// 		}
	// 	}
	// 	else {
	// 		size_t		pos_space;
	// 		size_t		pos_space2;
	// 		size_t		pos_semicolon;
	// 		std::string	key;
	// 		std::string	val;

	// 		pos_space = tmpOneLine.find(" ");
	// 		pos_space2 = tmpOneLine.find(" ", pos_space + 1);
	// 		pos_semicolon = tmpOneLine.find_last_of(";");

	// 		key = tmpOneLine.substr(0, pos_space2 - 0);

	// 		// val = tmpOneLine.substr(pos_space2 + 1, (pos_semicolon - pos_space2) - 1);
	// 		val = tmpOneLine.substr(pos_space2 + 1, (pos_semicolon - pos_space2));

	// 		// std::cout << "key " << key << std::endl;
	// 		// std::cout << GRN << "val " << val << reset << std::endl;

	// 		this->_detail.insert(std::pair<std::string, std::string>(key, val));
	// 	}
	// }
}

// void	ServConfigDetail::storeConfig(std::string const & config) {
// 	size_t		pos_space;
// 	size_t		pos_semicolon;
// 	std::string	key;
// 	std::string	val;

// 	pos_space = config.find(" ");
// 	pos_semicolon = config.find(";");

// 	if ((pos_space == std::string::npos) || (pos_semicolon == std::string::npos)) {
// 		std::cout << RED << "[ERROR]Format of config file error" << reset << std::endl;
// 		if (pos_space == std::string::npos)
// 			std::cout << BLU << "[ERROR]Not found space" << reset << std::endl;
// 		if (pos_semicolon == std::string::npos)
// 			std::cout << BLU << "[ERROR]Not found semicolon" << reset << std::endl;
// 		return;
// 	}
// 	else {
// 		// std::cout << RED << config << reset << std::endl;
// 		// std::cout << GRN << "Key @" << pos_space << " " << "val @" << pos_semicolon << reset << std::endl;

// 		key = config.substr(0, pos_space - 0);
// 		while (isblank(key[0])) {
// 			key.erase(0, 1);
// 		}
// 		val = config.substr(pos_space + 1, (pos_semicolon - pos_space) - 1);

// 		// std::cout << "key " << key << std::endl;
// 		// std::cout << GRN << "val " << val << reset << std::endl;

// 		this->_detail.insert(std::pair<std::string, std::string>(key, val));
// 	}
// }

void ServConfigDetail::showDetail(void)
{
	std::multimap<std::string, std::string>::iterator it = this->_detail.begin();
	while (it != this->_detail.end())
	{
		std::cout << MAG << it->first << ":" << it->second << reset << std::endl;
		it++;
	}
}

std::string const &ServConfigDetail::getVal(std::string const &val)
{
	std::multimap<std::string, std::string>::iterator it;

	std::cout << MAG << "[INFO]ServConfigDetail getVal" << reset << std::endl;
	if (val == "location_back /cgi_bins/.py" && this->_detail.find(val) == this->_detail.end())
	{
		printMap(this->_detail);
	}
	it = this->_detail.find(val);
	if (it != this->_detail.end())
	{
		std::cout << MAG << "[INFO]Found key" << reset << std::endl;
		std::cout << YEL << "val: " << val << reset << std::endl;
		return (it->second);
	}
	else
	{
		this->_tmpStr.clear();
		std::cout << RED << "val is " << val << reset << std::endl;
		std::cout << RED << "[INFO]NOT Found key:" << this->_tmpStr << reset << std::endl;
		// return string that doesn't use
		return (this->_tmpStr);
	}
}

void ServConfigDetail::delFrontSpace(std::string *str)
{
	// std::cout << CYN << *str << reset;
	int i = 0;

	// while (isspace(this->_tmpStr[i])) {
	while (isspace((*str)[i]))
	{
		i++;
	}
	(*str).erase(0, i);
	// std::cout << BLU << *str << reset;
}

void ServConfigDetail::delAllComment(std::string *str)
{
	// if ((*str).find("#") != std::string::npos) {
	// 	std::cout << "Del All comment" << (*str).find("#") << std::endl;
	// 	std::cout << "Del All comment" << (*str).find("\n", (*str).find("#")) << std::endl;
	// }
	while ((*str).find("#") != std::string::npos)
	{
		(*str).erase((*str).find("#"), (*str).find("\n", (*str).find("#")) - (*str).find("#"));
	}
}

void ServConfigDetail::storeOneLineNormalConfig(std::string const &str)
{
	int pos_space;
	std::string key;
	std::string val;
	std::string tmpStore = str.substr(0, str.find(";"));
	// std::cout << MAG << tmpStore << reset << std::endl;

	this->delIsCntrl(&tmpStore);
	pos_space = tmpStore.find(" ");
	key = tmpStore.substr(0, pos_space);
	val = tmpStore.substr(pos_space, tmpStore.size() - pos_space);
	this->delFrontSpace(&val);
	// std::cout << "key:" << key << MAG << "test msg" << reset << std::endl;
	// std::cout << "val:" << val << std::endl;
	this->_detail.insert(std::pair<std::string, std::string>(key, val));
}

void ServConfigDetail::storeLocationConfig(std::string const &str)
{
	int pos_space;
	std::string key;
	std::string val;
	std::string tmpStore = str.substr(0, str.find("}"));

	tmpStore.erase(tmpStore.find("{"), 1);

	pos_space = tmpStore.find(" ");
	// std::cout << "space1 " << pos_space << std::endl;
	pos_space = tmpStore.find(" ", pos_space + 1);
	// std::cout << "space1 " << pos_space << std::endl;
	key = tmpStore.substr(0, pos_space);
	val = tmpStore.substr(pos_space, tmpStore.size() - pos_space);
	// std::cout << "len bef del front space " << tmpStore.size() - pos_space << std::endl;
	this->delFrontSpace(&val);
	// std::cout << "key:" << key << MAG << "test msg" << reset << std::endl;
	// std::cout << "val:" << val << std::endl;
	// std::cout << "len val:" << val.size() << std::endl;
	this->delIsCntrl(&val);
	if (key == "location_back /cgi_bins/.py")
		std::cout << "xxxkey:" << key << MAG << "test msg" << reset << std::endl;
	// std::cout << "xxxval:" << val << std::endl;
	//  if (key == "location_back /cgi_bins/.py")
	//  	exit(0);
	this->_detail.insert(std::pair<std::string, std::string>(key, val));

	// std::cout << GRN << tmpStore << reset;
}

void ServConfigDetail::delIsCntrl(std::string *str)
{
	// while ((*str).find("\n") != std::string::npos) {
	// 	(*str).erase((*str).find("\n"), 1);
	// }

	int i = (*str).size();
	// std::cout << "del new line" << std::endl;
	// std::cout << "len " << (*str).size() << std::endl;

	// // while (*str != std::string::npos) { // not working
	// while (*str != "\0") {
	// 	std::cout << *str << std::endl;
	// 	str++;
	// }
	// std::cout << (*str);
	while (i > 0)
	{
		i--;
		if (iscntrl((*str)[i]))
			str->erase(i, 1);
	}
	// std::cout << "aft del iscntrl" << std::endl;
	// std::cout << (*str) << std::endl;
	// std::cout << "end print new line" << std::endl;
}

// void	ServConfigDetail::storeConfigLocation(std::string const & location) {
// 	if (location.find("{") != std::string::npos) {
// 		std::cout << "Found open new location" << std::endl;
// 		// std::cout << MAG << "detail " << location << reset << std::endl;

// 		// LocationConfigDetail	*tmpLocation = new LocationConfigDetail();
// 		// this->_locationDetail[this->_amountLocation] = new LocationConfigDetail();
// 		// std::cout << "Size of location bef add " << this->getAmountLocation() << std::endl;
// 		// this->_locationDetail.insert(this->_amountLocation, new LocationConfigDetail);
// 		this->_locationDetail.insert(std::pair<int, LocationConfigDetail *>(this->_amountLocation, new LocationConfigDetail()));
// 		// std::cout << "Size of location aft add " << this->getAmountLocation() << std::endl;
// 	}
// 	else if (location.find("}") != std::string::npos) {
// 		std::cout << "Found close new location" << std::endl;
// 	}
// 	else {
// 		std::cout << MAG << "detail " << location << reset << std::endl;
// 	}
// }

// int		ServConfigDetail::getAmountLocation(void) {
// 	return (this->_locationDetail.size());
// }

// std::string const &	ServConfigDetail::getValLocation(int index, std::string const & key) {
// }
