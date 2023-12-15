#include "ConfigFileHandle.hpp"
#include <sys/types.h>

ConfigFileHandle::ConfigFileHandle(void)
{
	this->_tmpServConfigDetail = NULL;
	this->_amountServConfigDetail = 0;
	this->_tmpStr.clear();
	// this->_foundLocation = 0;
}

ConfigFileHandle::~ConfigFileHandle(void)
{
	std::map<unsigned int, ServConfigDetail *>::iterator iter = this->_servConfig.begin();
	while (iter != this->_servConfig.end())
	{
		delete iter->second;
		iter++;
	}
}

int ConfigFileHandle::readConfigFile(char const *fileName)
{
	// std::cout << MAG << "read Server Config File" << reset << std::endl;
	std::ifstream ifs;

	std::string tmpRd;

	ifs.open(fileName, std::ifstream::in);
	if (ifs.good() == false)
	{
		ifs.close();
		std::cout << RED << fileName << " file not found or no permission. please input valid filename" << reset << std::endl;
		// throw !!!!!!!!!!!!!!!!!!!!!!1
		return (1);
	}
	else
	{
		int cntOpenBlacket = 0;
		int cntCloseBlacket = 0;
		int sndBlockConfig = 0;
		int foundServKeyW = 0;
		std::string tmpOneBlockServConfig;

		tmpOneBlockServConfig.clear();

		while (std::getline(ifs, tmpRd))
		{
			// new code
			if (tmpRd.find("server") != std::string::npos)
			{
				foundServKeyW = 1;
			}
			if (tmpRd.find("{") != std::string::npos)
			{
				cntOpenBlacket++;
			}
			if (tmpRd.find("}") != std::string::npos)
			{
				cntCloseBlacket++;
			}
			if (foundServKeyW == 1)
			{
				tmpOneBlockServConfig += tmpRd += "\n";
			}

			if ((cntCloseBlacket == cntOpenBlacket) && (foundServKeyW == 1) && (cntOpenBlacket > 0))
			{
				this->_servConfig[this->_servConfig.size()] = new ServConfigDetail();
				this->_tmpServConfigDetail = this->_servConfig[this->_servConfig.size() - 1];
				this->_tmpServConfigDetail->saveOneBlockConfig(tmpOneBlockServConfig);

				cntOpenBlacket = 0;
				cntCloseBlacket = 0;
				sndBlockConfig = 0;
				foundServKeyW = 0;
				tmpOneBlockServConfig.clear();
				this->_amountServConfigDetail++;
			}
			// std::cout << YEL << "tmpRd as below" << reset << std::endl;
			// std::cout << YEL << tmpRd << reset << std::endl;
		}
		ifs.close();
	}
	return (0);
}

void ConfigFileHandle::showDetailConfigFile(void)
{
	for (int i = 0; i < this->_amountServConfigDetail; i++)
	{
		this->_servConfig[i]->showDetail();
		std::cout << std::endl;
	}
}

unsigned int ConfigFileHandle::getAmountServConfig(void)
{
	return (this->_amountServConfigDetail);
}

std::string const &ConfigFileHandle::getServConfigVal(int servNum, std::string const &key)
{
	std::map<unsigned int, ServConfigDetail *>::iterator it;

	std::cout << MAG << "[INFO]configFileHandle getServConfigVal" << reset << std::endl;
	// std::cout << GRN << "key in getServConfigVal(): " << key << reset << std::endl;
	it = this->_servConfig.find(servNum);
	if (it != this->_servConfig.end())
	{
		std::cout << CYN << "key: " << key << reset << std::endl;
		return (it->second->getVal(key));
	}
	else
	{
		std::cout << CYN << "key: " << key << reset << std::endl;
		this->_tmpStr.clear();
		return (this->_tmpStr);
	}
}
