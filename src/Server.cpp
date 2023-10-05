#include "Server.hpp"

Server::Server(void) {
	this->_ip.clear();
	this->_port.clear();
	this->_hostname.clear();
}

Server::~Server(void) {}

void	Server::setServSockAddr(std::string const & config) {
	this->parseSockAddr(config);
}

struct sockaddr_in	Server::getServSockAddr(void) {
	return (this->_address);
}

std::string const &	Server::getIP(void) {
	return (this->_ip);
}

std::string const &	Server::getPort(void) {
	return (this->_port);
}

std::string const & Server::getHostname(void) {
	return (this->_hostname);
}

int const &			Server::getFd(void) {
	return (this->_fd);
}

int	Server::servCreate(void) {
	int	opt = 1;

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(atoi(this->_port.c_str()));

	if ((this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
		perror("socket failed");
		return (EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		return (EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if (bind(this->_fd, (struct sockaddr *)&(this->_address), sizeof(struct sockaddr)) < 0) {
	// if (bind(this->_fd, static_cast<struct sockaddr *>(&(this->_address)), sizeof(struct sockaddr)) < 0) {
		perror("bind failed");
		return (EXIT_FAILURE);
	}

	// changing socket from active to passive
	if (listen(this->_fd, 1024) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return (0);
}

int	Server::servClose() {
	return (0);
}

void	Server::parseSockAddr(std::string const & config) {
	std::cout << MAG << "[INFO]Parsing " << config << reset << std::endl;
	if (config.find(":") == std::string::npos) {
		// Not define IP addr
		// std::cout << MAG << "Not found IP" << reset << std::endl;
		// std::cout << MAG << "Port num " << atoi(config.c_str()) << std::endl;
		this->_port = config;
	}
	// else {
	// 	// define IP addr
	// 	std::cout << MAG << "found IP" << reset << std::endl;
	// }
}
