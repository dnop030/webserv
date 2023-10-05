#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <string>

#include "color.hpp"

#include <cstdlib>

#include <netinet/in.h>
// #include <stdlib.h>
// #include <sys/socket.h>

class Server
{
	public:
		Server(void);
		~Server(void);

		void				setServSockAddr(std::string const & config);

		struct sockaddr_in	getServSockAddr(void);
		std::string const &	getIP(void);
		std::string const &	getPort(void);
		std::string const & getHostname(void);
		int const &			getFd(void);

		int					servCreate(void);
		int					servClose();

	private:
		Server(Server const & r);
		Server	&operator=(Server const & r);

		void	parseSockAddr(std::string const & config);

		struct sockaddr_in	_address;
		std::string			_ip;
		std::string			_port;
		std::string			_hostname;
		int 				_fd;
};

#endif
