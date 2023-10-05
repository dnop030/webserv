#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP

#include <iostream>
#include <string>

#include <sys/socket.h>
// #include <vector>
#include <map>

// test epool
#include "sys/epoll.h"

// socket
#include "netinet/in.h"
#include "sys/socket.h"

// errno
#include "errno.h"

// for generate response
#include <sstream>

#include "ConfigFileHandle.hpp"

// #include "stdio.h"

#define MAXEVENTS 128
#define BUFFPACK 2048

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

		void	createServ(std::string const & config);
		// int		findVec(std::vector<int> const & vec, int val);
		void	showMapFd(void);

		void	sockServRd(int const & servFd);
		void	sockServWr(void);
		void	sockCliRd(int const & cliFd);
		void	sockCliWr(void);

		std::string	generateHttpResponse(int statusCode, std::string const & statusMessage, std::string const & content);

		ConfigFileHandle		_configServ;
		// std::vector<Server *>	_serv;
		// Server 					*_tmpServer;

		// using only when create socket server
		// std::vector<int>		_servFd;
		std::map<int, char>		_mapFd;

		struct sockaddr_in		_address;
		int						_fd;

		int						_epoll_fd;
		struct epoll_event		_event;
		struct epoll_event		*_event_ret;

		// std::vector<int>		_cliFd;
		struct sockaddr			_inAddr;
		socklen_t				_inLen;
		int						_infd;
		char					_buffRd[BUFFPACK];
		std::string				_bufferPack;
		int						_valRd;
		std::string				_bufferSend;

		int						_tmpInt;
		std::string				_tmpStdStr;
};

#endif
