#ifndef SERVERHANDLE_HPP
#define SERVERHANDLE_HPP

#include <iostream>
#include <string>

#include <sys/socket.h>
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

#include <fcntl.h>

// #include "signal.h"
// #include <csignal>

#include "unistd.h"

#include "ConfigFileHandle.hpp"
#include "HttpHandle.hpp"

#define MAXEVENTS 128
#define BUFFPACK 2048

class ServHandle
{
public:
	ServHandle(void);
	~ServHandle(void);

	void servCreate(char const *configFile);
	void servStart(void);
	void servStop(void);

private:
	ServHandle(ServHandle const &r);
	ServHandle &operator=(ServHandle const &r);

	void createServ(std::string const &config);
	void showMapFd(void);

	void sockServRd(int const &servFd);
	void sockServWr(int const &servFd);
	void sockCliRd(int const &cliFd);
	void sockCliWr(int const &cliFd);

	void closeSock(int fd);

	std::string generateHttpResponse(int statusCode, std::string const &statusMessage, std::string const &content);

	void ConnectionHandle(HttpHandle &http, int const &cliFd);
	void ConnectionClose(void);

	bool _servRunning;

	// ConfigFileHandle			_configServ;
	ConfigFileHandle *_configServ;

	// using only when create socket server
	std::map<int, std::string> _mapFd;

	struct sockaddr_in _address;
	int _fd;

	int _epoll_fd;
	struct epoll_event _event;
	struct epoll_event *_event_ret;

	struct sockaddr _inAddr;
	socklen_t _inLen;
	int _infd;
	char _buffRd[BUFFPACK];
	std::string _bufferPack;
	int _valRd;
	std::string _bufferSend;
	std::map<int, std::string> _httpRespose;

	int _tmpInt;
	std::string _tmpStdStr;
};

#endif
