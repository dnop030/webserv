#ifndef SERVERHANDLE_HPP
# define SERVERHANDLE_HPP

# include <iostream>
# include <string>
# include "color.hpp"

# include "configFile.hpp"

class ServHandle
{
	public:
		ServHandle(void);
		~ServHandle(void);

		void	servCreate(const char * configFile);
		void	servStart(void);
		void	servStop(void);

	private:
		ServHandle(ServHandle const & r);
		ServHandle &operator=(ServHandle const & r);

		configFile	_configFileServer;
};

#endif
