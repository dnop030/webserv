#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP

#include <iostream>
#include <string>

class ServHandle
{
	public:
		ServHandle(void);
		~ServHandle(void);

		void	servCreate(std::string const & confFile);
		void	servStart(void);
		void	servStop(void);

	private:
		ServHandle(ServHandle const & r);
		ServHandle &operator=(ServHandle const & r);
}

#endif
