#include "ServHandle.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

ServHandle::ServHandle(void) {}

ServHandle::~ServHandle(void) {}

void	ServHandle::servCreate(char const * configFile) {
	std::cout << MAG << "servCreate" << reset << std::endl;
	this->_configServ.readConfigFile(configFile);

	// // Debug
	// this->_configServ.showDetailConfigFile();

	// std::string	tmpStr;
	// tmpStr = this->_configServ.getServConfigVal(0, "listen");
	// std::cout << MAG << "[INFO]output \"listen\" 0 " << tmpStr << reset << std::endl;

	// tmpStr = this->_configServ.getServConfigVal(0, "listen ");
	// std::cout << MAG << "[INFO]output \"listen \" 0 " << tmpStr << reset << std::endl;
	// // std::cout << MAG << "[INFO]output length " << tmpStr.length() << reset << std::endl;

	this->showMapFd();

	std::cout << MAG << "amout of serv config " << this->_configServ.getAmountServConfig() << reset << std::endl;
	for (int i=0; i<this->_configServ.getAmountServConfig(); i++) {
		this->createServ(this->_configServ.getServConfigVal(i, "listen"));
		// std::cout << MAG << "[INFO]fd of server " << this->_servFd[i] << reset << std::endl;
	}

	// epolll create
	// if flag = 0 => it is similar to epoll_create
	this->_epoll_fd = epoll_create1(0);
	if (this->_epoll_fd == -1) {
		std::cerr << RED << "Failed to create epoll file descriptor" << reset << std::endl;
		exit (EXIT_FAILURE);
	}

	// // Add Fd of server into epoll instance
	// for (int i=0; i<this->_configServ.getAmountServConfig(); i++) {
	// 	this->_event.data.fd = this->_servFd[i];
	// 	// event.events = EPOLLIN | EPOLLET;
	// 	this->_event.events = EPOLLIN | EPOLLOUT;
	// 	// event.events = EPOLLIN | EPOLLOUT | EPOLLET;

	// 	this->_tmpInt = epoll_ctl (this->_epoll_fd, EPOLL_CTL_ADD, this->_servFd[i], &(this->_event));
	// 	if (this->_tmpInt < 0) {
	// 		std::cerr << "Failed to add server file descriptor in instance epoll of server " << i << std::endl;
	// 	}
	// }
	// Add Fd of server into epoll instance
	for (std::map<int, char>::iterator it = this->_mapFd.begin(); it != this->_mapFd.end(); it++) {
		this->_event.data.fd = it->first;
		// event.events = EPOLLIN | EPOLLET;
		this->_event.events = EPOLLIN | EPOLLOUT;
		// event.events = EPOLLIN | EPOLLOUT | EPOLLET;

		this->_tmpInt = epoll_ctl (this->_epoll_fd, EPOLL_CTL_ADD, it->first, &(this->_event));
		if (this->_tmpInt < 0) {
			std::cerr << RED << "Failed to add server file descriptor in instance epoll of server " << it->first << reset << std::endl;
		}
	}

	// Show all Server Fd
	// std::cout << MAG << "Show serv Fd" << reset << std::endl;
	// for (int i=0; i<this->_configServ.getAmountServConfig(); i++) {
	// 	std::cout << this->_servFd[i] << std::endl;
	// }
	this->showMapFd();

	// Need to fix -> using new method
	this->_event_ret = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
}

void	ServHandle::servStart(void) {

	while (1) {
		int	numEvent, i;

		std::cout << std::endl << YEL << "Wait for Event" << reset << std::endl;
		numEvent = epoll_wait (this->_epoll_fd, this->_event_ret, MAXEVENTS, -1);
		std::cout << std::endl << MAG << "[INFO]found " << numEvent << " event(s)" << reset << std::endl;

		for (i=0; i<numEvent; i++) {
			std::cout << MAG << "[INFO]flag " << this->_event_ret[i].events << " to handle" << reset << std::endl;
			if ((this->_event_ret[i].events & EPOLLERR) || (this->_event_ret[i].events & EPOLLHUP)) {

				this->closeSock(this->_event_ret[i].data.fd);

				/* An error has occured on this fd, or the socket is not
				ready for reading (why were we notified then?) */
				std::cerr << RED << "[ERROR] epoll error" << reset << std::endl;
				std::cerr << RED << "[ERROR] error @ fd tmpFd " << this->_event_ret[i].data.fd << reset << std::endl;
			}
			else {
				// Always Rd before Wr due to every Rd package need to be Sendback
				// whether the package is good request or bad request
				if (this->_event_ret[i].events & EPOLLIN) {
					std::cout << MAG << "[INFO] Found Rd on " << this->_event_ret[i].data.fd << reset << std::endl;
					std::map<int, char>::iterator	it;

					it = this->_mapFd.find(this->_event_ret[i].data.fd);
					if (it != this->_mapFd.end()) {

						// cannot using keyword after actioun Rd
						// Due to every Rd packages
						if (it->second == 's') {
							// go to server Rd Accept
							this->sockServRd(it->first);
							std::cout << std::endl << YEL << "End sockSerRd" << reset << std::endl;

							// // Due to divide action only Rd
							// continue;
						}
						else {
							// go to Rd Request
							this->sockCliRd(it->first);
							std::cout << std::endl << YEL << "End sockCliRd" << reset << std::endl;

							// // Due to divide action only Rd
							// continue;
						}

						// // Due to divide action only Rd
						// continue;
					}
					else {
						std::cout << RED << "[ERROR] fake fd num " << this->_event_ret[i].data.fd << reset << std::endl;
					}

					std::cout << std::endl << MAG << "[INFO]event flag " << this->_event_ret[i].events << " on " << this->_event_ret[i].data.fd << " fd after Rd"  << reset << std::endl;
				}
				if ((this->_event_ret[i].events & EPOLLOUT) && (this->_httpRespose.size() > 0)) {
					std::cout << MAG << "[INFO] Found Wr on " << this->_event_ret[i].data.fd << reset << std::endl;
					std::map<int, char>::iterator	it;

					it = this->_mapFd.find(this->_event_ret[i].data.fd);
					std::cout << std::endl << YEL << "Result Fd for Wr is " << it->first << std::endl;
					if (it != this->_mapFd.end()) {
						std::cout << std::endl << YEL << "Fd Bef Wr is " << it->first << std::endl;
						if (it->second == 's') {
							// go to server Wr What???
							this->sockServWr(it->first);
							std::cout << std::endl << YEL << "End sockSerWr" << reset << std::endl;
							// continue;
						}
						else {
							// go to Wr Response
							this->sockCliWr(it->first);
							std::cout << std::endl << YEL << "End sockCliWr" << reset << std::endl;
							// continue;
						}

						std::cout << MAG << "[INFO]finish Wr" << reset << std::endl;
						// forget to del response after Wr ?????????????????????????????????????
						std::map<int, std::string>::iterator	itHttpResponse = this->_httpRespose.find(it->first);
						if (itHttpResponse != this->_httpRespose.end()) {
							std::cout << YEL << "Del Response" << std::endl << itHttpResponse->second << reset << std::endl << std::endl;
							this->_httpRespose.erase(itHttpResponse);
						}
						else {
							std::cout << YEL << "Not found response from " << it->first << reset << std::endl;
						}

						// Due to divide action only Wr
						continue;
					}
					else {
						std::cout << RED << "[ERROR] fake fd num " << this->_event_ret[i].data.fd << reset << std::endl;
					}

					std::cout << std::endl << MAG << "[INFO]event flag " << this->_event_ret[i].events << " on " << this->_event_ret[i].data.fd << " fd after Wr"  << reset << std::endl;
				}
			}
		}
	}
}

void	ServHandle::servStop(void) {}

void	ServHandle::createServ(std::string const & config) {
	int	opt = 1;
	// std::vector<int>::iterator	it = this->_servFd.begin();
	std::map<int, char>::iterator	it = this->_mapFd.begin();

	// std::cout << MAG << "config " << config << reset << std::endl;

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(atoi(config.c_str()));

	if ((this->_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
	// if ((this->_fd = socket(AF_INET, SOCK_NONBLOCK, 0)) < 0) {
		perror("socket failed");
		exit (EXIT_FAILURE);
	}
	// std::cout << YEL << "socket fd "<< this->_fd << reset << std::endl;

	// Forcefully attaching socket to the port
	if (setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit (EXIT_FAILURE);
	}
	// std::cout << YEL << "socket opt" << reset << std::endl;

	// Forcefully attaching socket to the port
	if (bind(this->_fd, (struct sockaddr *)&(this->_address), sizeof(struct sockaddr)) < 0) {
	// if (bind(this->_fd, static_cast<struct sockaddr *>(&(this->_address)), sizeof(struct sockaddr)) < 0) {
		perror("bind failed");
		exit (EXIT_FAILURE);
	}
	// std::cout << YEL << "bind" << reset << std::endl;

	// changing socket from active to passive
	if (listen(this->_fd, 1024) < 0) {
		perror("listen");
		exit (EXIT_FAILURE);
	}

	// // Add Fd into vector
	// this->_servFd.insert(it, this->_fd);

	// Add Fd into map
	it = this->_mapFd.find(this->_fd);
	if (it == this->_mapFd.end()) {
		this->_mapFd.insert(std::pair<int, char>(this->_fd, 's'));
	}
	else {
		std::cerr << std::endl << RED << "[ERROR] Found duplicate fd" << reset << std::endl;
	}
}

void	ServHandle::showMapFd(void) {
	std::map<int, char>::iterator	it = this->_mapFd.begin();

	std::cout << std::endl << "Show Map Fd" << std::endl;
	// std::cout << "fd s/c" << std::endl;
	while (it != this->_mapFd.end()) {
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}
	std::cout << "-----------" << std::endl;
	std::cout << std::endl;
}

void	ServHandle::sockServRd(int const & servFd) {
	std::map<int, char>::iterator	it;
	// this->_inLen = sizeof(this->_inAddr);
	// this->_infd = accept(this->_servFd[this->_tmpInt], &(this->_inAddr), &(this->_inLen));
	this->_infd = accept(servFd, NULL, NULL);
	if (this->_infd == -1) {
		if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
			/* We have processed all incoming
				connections. */
			// need to be handle ???????????????????????????
			return ;
		}
		else {
			perror ("accept");
			return ;
		}
	}

	// Make the accepted socket non-blocking
	// int flags = fcntl(this->_infd, F_GETFL, 0);
	// if (flags == -1) {
	// 	perror("fcntl get");
	// 	close(this->_infd);
	// }
	// if (fcntl(this->_infd, F_SETFL, flags | O_NONBLOCK) == -1) {
	if (fcntl(this->_infd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl set");
		close(this->_infd);
	}

	// register new socket to epoll
	this->_event.data.fd = this->_infd;
	this->_event.events = EPOLLIN | EPOLLOUT | EPOLLET;
	this->_tmpInt = epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, this->_infd, &(this->_event));
	if (this->_tmpInt == -1) {
		perror("epoll_ctl accept");
		abort();
	}

	// Add client Fd into map
	// this->_cliFd.insert(this->_cliFd.begin(), this->_infd);
	it = this->_mapFd.find(this->_infd);
	if (it == this->_mapFd.end()) {
		this->_mapFd.insert(std::pair<int, char>(this->_infd, 'c'));
	}
	else {
		std::cerr << std::endl << RED << "[ERROR] Found duplicate fd when accept" << reset << std::endl;
	}

	// for testing accept function
	this->showMapFd();
}

void	ServHandle::sockServWr(int const & servFd) {
	std::cout << YEL << "[WARNING] Found some package on Server socket Need to send back" << reset << std::endl;
}

void	ServHandle::sockCliRd(int const & cliFd) {
	this->_bufferPack.clear();
	this->_tmpStdStr.clear();
	this->_buffRd[0] = '\0';
	do {
		// Recv Work but no Wr Event
		// // this->_valRd = recv(this->_infd, this->_buffRd, sizeof(this->_buffRd), 0);
		// // this->_valRd = read(this->_infd, this->_buffRd, sizeof(this->_buffRd));
		// this->_valRd = recv(this->_infd, this->_buffRd, BUFFPACK, 0);
		// std::cout << "Recv " << this->_valRd << std::endl;
		// if (this->_valRd > 0) {
		// 	// printf("\n\n%s\n\n", this->_buffRd);
		// 	// std::cout << this->_buffRd << std::endl;
		// 	this->_tmpStdStr = this->_buffRd;
		// 	this->_bufferPack += this->_tmpStdStr;
		// 	// this->_bufferPack.append(this->_buffRd, this->_valRd);
		// }

		this->_valRd = recv(this->_infd, this->_buffRd, BUFFPACK, 0);
		// std::cout << "Recv " << this->_valRd << std::endl;
		if (this->_valRd > 0) {
			// printf("\n\n%s\n\n", this->_buffRd);
			// std::cout << this->_buffRd << std::endl;
			this->_tmpStdStr = this->_buffRd;
			// this->_bufferPack += this->_tmpStdStr;
			this->_bufferPack.append(this->_tmpStdStr, this->_valRd);
			// this->_bufferPack.append(this->_tmpStdStr, this->_valRd + 1);
		}
	}
	while (this->_valRd > 0);
	std::cout << CYN << "valRd " << this->_valRd << reset << std::endl;

	// if (this->_valRd == -1) {
	// 	// Handle error if needed
	// 	perror("recv");
	// }

	// std::cout << CYN << "Data in Package tmpStdStr" << reset << std::endl;
	// std::cout << CYN << this->_tmpStdStr << reset << std::endl << std::endl;

	std::cout << CYN << "Data in Package bufferPack" << reset << std::endl;
	std::cout << CYN << this->_tmpStdStr << reset << std::endl << std::endl;

	if (this->_tmpStdStr.size() > 0) {
		// prepare the response and tie with client Fd
		if (this->_httpRespose.find(cliFd) == this->_httpRespose.end()) {
			this->_httpRespose.insert(std::pair<int, std::string>(cliFd, this->generateHttpResponse(200, "Ok", "Hello from server")) );
		}
		else {
			std::cout << YEL << "[WARNING] Need to handle Fd receive multiple request" << reset << std::endl;
		}
	}
	else {
		// if the size of receive package = 0
		// means client send some Flag ex. FIN
		this->closeSock(cliFd);
	}

	// std::cout << CYN << "call from Rd" << reset << std::endl;
	// this->sockCliWr(cliFd);

	// show response after receive request
	std::cout << std::endl << "Show Response in CliRd" << std::endl;
	{
		std::map<int, std::string>::iterator	iterResponse = this->_httpRespose.begin();

		while (iterResponse != this->_httpRespose.end()) {
			std::cout << iterResponse->first << " " << iterResponse->second << std::endl;
			iterResponse++;
		}
		std::cout << "-------------" << std::endl << std::endl;
	}
}

void	ServHandle::sockCliWr(int const & cliFd) {
	std::cout << std::endl << CYN << "In sockCliWr" << reset << std::endl;
	std::cout << YEL << "Wr for socket " << cliFd << reset << std::endl;
	std::map<int, std::string>::iterator	it = this->_httpRespose.end();

	it = this->_httpRespose.find(cliFd);
	if (it != this->_httpRespose.end()) {
		// send(int fd, const void *buf, size_t n, int flags)
		send(cliFd, it->second.c_str(), it->second.size(), 0);
	}
	else {
		std::cout << YEL << "[WARNING] Not found response for " << cliFd << reset << std::endl;
	}
}

void	ServHandle::closeSock(int fd) {

	// Delete response when close socket
	// Therefore, socket can be reuse
	std::map<int, std::string>::iterator	itHttpResponse = this->_httpRespose.find(it->first);
	if (itHttpResponse != this->_httpRespose.end()) {
		std::cout << YEL << "Del Response when close socket" << std::endl << itHttpResponse->second << reset << std::endl << std::endl;
		this->_httpRespose.erase(itHttpResponse);
	}

	//forget to clear fd _mapFd
	// std::map<int, char>::iterator	it = this->_mapFd.find(this->_event_ret[i].data.fd);
	std::map<int, char>::iterator	it = this->_mapFd.find(fd);
	this->_tmpInt = it->first;
	if (it != this->_mapFd.end()) {
		std::cout << MAG << "[INFO] Delete Fd from mapFd" << reset << std::endl;
		this->_mapFd.erase(it);
	}

	// del Fd from EPOLL instance
	// this->_tmpInt = epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, this->_event_ret[i].data.fd, NULL);
	this->_tmpInt = epoll_ctl(this->_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
	if (this->_tmpInt != 0) {
		perror("epoll_ctl(EPOLL_CTL_DEL)");
	}

	// close(this->_event_ret[i].data.fd);
	this->_tmpInt = close(fd);
	if (this->_tmpInt != 0) {
		perror("close when Err ");
	}

}

std::string	ServHandle::generateHttpResponse(int statusCode, std::string const & statusMessage, std::string const & content) {
	// Create an output stream to build the response
	std::ostringstream responseStream;

	// HTTP status line
	responseStream << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";

	// Headers
	responseStream << "Content-Length: " << content.length() << "\r\n";
	responseStream << "Content-Type: text/plain\r\n"; // Adjust content type as needed
	responseStream << "\r\n"; // End of headers

	// Response content
	responseStream << content;

	return responseStream.str();
}
