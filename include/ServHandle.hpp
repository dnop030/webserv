#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP

#include <iostream>
#include <string>

#include <vector>

#include "ConfigFileHandle.hpp"
#include "Server.hpp"

class ServHandle
{
	public:
		ServHandle(void);
		~ServHandle(void);

		void	servCreate(char const * configFile);
		void	servStart(void);
		void	servStop(void);

	private:
		ServHandle(ServHandle const & r);
		ServHandle &operator=(ServHandle const & r);

		ConfigFileHandle		_configServ;
		std::vector<Server *>	_serv;
		Server 					*_tmpServer;
};

#endif
