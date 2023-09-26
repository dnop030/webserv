#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP

#include <iostream>
#include <string>

#include "ConfigFileHandle.hpp"

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

		ConfigFileHandle	_configServ;
};

#endif
