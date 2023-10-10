// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAXEVENTS 64

// test getaddrinfo
#include "sys/types.h"
#include "netdb.h"

// test epool
// #include "sys/epoll.h"

// include errno
#include "errno.h"

// test include fcntl need to find solution later *********************
#include "fcntl.h"

#include "limits.h"

// #include <string>

#include "Request.hpp"

// static int
// make_socket_non_blocking(int sfd)
// {
// 	int flags, s;

// 	flags = fcntl(sfd, F_GETFL, 0);
// 	if (flags == -1)
// 	{
// 		perror("fcntl");
// 		return -1;
// 	}

// 	flags |= O_NONBLOCK;
// 	s = fcntl(sfd, F_SETFL, flags);
// 	if (s == -1)
// 	{
// 		perror("fcntl");
// 		return -1;
// 	}

// 	return 0;
// }

// int createsocketNBind(struct sockaddr_in *address)
// {
// 	int server_fd;
// 	int opt = 1;

// 	// // Creating socket file descriptor
// 	// if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 	// For non-blocking creation socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0)
// 	{
// 		perror("socket failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Forcefully attaching socket to the port 8080
// 	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
// 	{
// 		perror("setsockopt");
// 		exit(EXIT_FAILURE);
// 	}
// 	// address->sin_family = AF_INET;
// 	// address->sin_addr.s_addr = INADDR_ANY;
// 	// address->sin_port = htons(PORT);

// 	// Forcefully attaching socket to the port 8080
// 	// if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
// 	if (bind(server_fd, (struct sockaddr *)address, sizeof(struct sockaddr)) < 0)
// 	{
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	return (server_fd);
// }

// int main(int argc, char const *argv[])
// {
// 	(void)argc;
// 	(void)argv;
// 	// int					server_fd = 0, new_socket = 0, valread;
// 	// int					server_fd = 0, new_socket = 0, valread, tmpint;
// 	int server_fd0 = 0, server_fd1 = 0, tmpint;
// 	struct sockaddr_in address;
// 	// int					opt = 1;
// 	// int					addrlen = sizeof(address);
// 	// char				buffer[1024] = { 0 };
// 	// char				hello[50] = "Hello from server";
// 	// char				responseOkPage[4096] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 50\n\n<!DOCTYPE html>\n<html>\n<head>\n\t<title>200 OK</title>\n</head>\n<body>\n\t<h1>Success! The request was processed.</h1>\n</body>\n</html>\n";
// 	// char				responseOkPage[4096] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 50\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>200 OK</title>\n</head>\n<body>\n<h1>Success! The request was processed.</h1>\n</body>\n</html>\n";
// 	// char				responseOkPage[4096] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>200 OK</title>\r\n</head>\r\n<body>\r\n<h1>Success! The request was processed.</h1>\r\n</body>\r\n</html>\r\n";
// 	char responseOkPage[4096] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!"; // this one work !!!!!!!!!!!!!!111

// 	int conn_cnt = 0; // for test clear when close server *****************

// 	int epoll_fd;
// 	struct epoll_event event;
// 	struct epoll_event *event_ret;

// 	int valread;
// 	char buffer[1024];

// 	// create epoll instance
// 	// epoll_fd = epoll_create(20000);
// 	epoll_fd = epoll_create1(0);

// 	if (epoll_fd == -1)
// 	{
// 		fprintf(stderr, "Failed to create epoll file descriptor\n");
// 		return 1;
// 	}

// 	// address.sin_family = AF_INET;
// 	// address.sin_addr.s_addr = INADDR_ANY;
// 	// address.sin_port = htons(PORT);

// 	int tmpport = 12345;

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(tmpport);

// 	server_fd0 = createsocketNBind(&address);
// 	printf("fd server0 = %d\n", server_fd0);

// 	// changing socket from active to passive
// 	// if (listen(server_fd0, 3) < 0) {
// 	if (listen(server_fd0, 1024) < 0)
// 	{
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	event.data.fd = server_fd0;
// 	// event.events = EPOLLIN | EPOLLET;
// 	// event.events = EPOLLIN;
// 	event.events = EPOLLIN | EPOLLOUT;

// 	tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd0, &event);
// 	if (tmpint < 0)
// 	{
// 		fprintf(stderr, "Failed to add server file descriptor in instance epoll\n");
// 	}

// 	tmpport = 12346;

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(tmpport);

// 	server_fd1 = createsocketNBind(&address);
// 	printf("fd server1 = %d\n", server_fd1);

// 	// changing socket from active to passive
// 	// if (listen(server_fd0, 3) < 0) {
// 	if (listen(server_fd1, 1024) < 0)
// 	{
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	event.data.fd = server_fd1;
// 	// event.events = EPOLLIN | EPOLLET;
// 	// event.events = EPOLLIN;
// 	event.events = EPOLLIN | EPOLLOUT;

// 	tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd1, &event);
// 	if (tmpint < 0)
// 	{
// 		fprintf(stderr, "Failed to add 2 server file descriptor in instance epoll\n");
// 	}

// 	event_ret = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));

// 	/* The event loop */
// 	while (1)
// 	{

// 		int n, i;

// 		printf("[INFO]bef epoll wait\n");
// 		// timeout = -1 is wait infinity, timeout = 0 is return immediately
// 		n = epoll_wait(epoll_fd, event_ret, MAXEVENTS, -1);
// 		printf("%d socket ready from epoll_wait\n\n\n", n);

// 		// testing by pond
// 		for (i = 0; i < n; i++)
// 		{
// 			if ((event_ret[i].events & EPOLLERR) || (event_ret[i].events & EPOLLHUP))
// 			{
// 				/* An error has occured on this fd, or the socket is not
// 					ready for reading (why were we notified then?) */
// 				fprintf(stderr, "epoll error\n");
// 				fprintf(stderr, "fd %d err\n", event_ret[i].data.fd);
// 				close(event_ret[i].data.fd);
// 			}
// 			else
// 			{
// 				if (server_fd0 == event_ret[i].data.fd)
// 				{
// 					/* We have a notification on the listening socket, which
// 					means one or more incoming connections. */
// 					struct sockaddr in_addr;
// 					socklen_t in_len;
// 					int infd;
// 					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

// 					printf("++++++++ [INFO] Serv Socket 1 Recv Data ++++++++\n");
// 					in_len = sizeof in_addr;
// 					infd = accept(server_fd0, &in_addr, &in_len);
// 					if (infd == -1)
// 					{
// 						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
// 						{
// 							/* We have processed all incoming
// 								connections. */
// 							// need to be handle ???????????????????????????
// 							break;
// 						}
// 						else
// 						{
// 							perror("accept");
// 							break;
// 						}
// 					}

// 					tmpint = getnameinfo(&in_addr, in_len,
// 										 hbuf, sizeof hbuf,
// 										 sbuf, sizeof sbuf,
// 										 NI_NUMERICHOST | NI_NUMERICSERV);
// 					if (tmpint == 0)
// 					{
// 						printf("Accepted connection on descriptor %d "
// 							   "(host=%s, port=%s)\n",
// 							   infd, hbuf, sbuf);
// 					}

// 					conn_cnt++; // for test clear when close server *****************
// 					printf("conn_cnt:%d\n", conn_cnt);

// 					// test read data in buffer
// 					// int		valread;
// 					// char	buffer[1024];
// 					valread = read(infd, buffer, 1024);
// 					// printf("valread is %s\n", valread);
// 					printf("line 259 num read:%d\n", valread);
// 					printf("output buffer %s\n", buffer);
// 					// if (valread < 0)
// 					// 	printf("[ERROR]Read Fn error\n");
// 					// else {
// 					// 	printf("[INFO]Read Fn ok\n");
// 					// 	// printf("Read Fn ok %c\n", buffer[0]);
// 					// 	// printf("Read Fn ok\n\tData: %s\n", buffer);
// 					// }

// 					// send(infd, &hello, strlen(hello), 0);
// 					if (event_ret[i].events & EPOLLIN)
// 					{
// 						// if (event_ret[i].events & EPOLLOUT) {
// 						printf("[INFO] Sending Response \n");
// 						send(infd, &responseOkPage, strlen(responseOkPage), 0);
// 						// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 					}

// 					// close(infd); // *************** test for other option

// 					// /* Make the incoming socket non-blocking and add it to the
// 					// 	list of fds to monitor. */
// 					// tmpint = make_socket_non_blocking (infd);
// 					// if (tmpint == -1)
// 					// 	abort ();

// 					event.data.fd = infd;
// 					// event.events = EPOLLIN | EPOLLET;
// 					event.events = EPOLLIN;
// 					// event.events = EPOLLIN | EPOLLET | EPOLLIN | EPOLLOUT;
// 					// event.events = EPOLLIN | EPOLLOUT; // error
// 					tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infd, &event);
// 					if (tmpint == -1)
// 					{
// 						perror("epoll_ctl accept");
// 						abort();
// 					}
// 				}
// 				else if (server_fd1 == event_ret[i].data.fd)
// 				{
// 					/* We have a notification on the listening socket, which
// 					means one or more incoming connections. */
// 					struct sockaddr in_addr;
// 					socklen_t in_len;
// 					int infd;
// 					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

// 					printf("++++++++ [INFO] Serv Socket 1 Recv Data ++++++++\n");
// 					in_len = sizeof in_addr;
// 					infd = accept(server_fd1, &in_addr, &in_len);
// 					if (infd == -1)
// 					{
// 						if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
// 						{
// 							/* We have processed all incoming
// 								connections. */
// 							// need to be handle ???????????????????????????
// 							break;
// 						}
// 						else
// 						{
// 							perror("accept");
// 							break;
// 						}
// 					}

// 					tmpint = getnameinfo(&in_addr, in_len,
// 										 hbuf, sizeof hbuf,
// 										 sbuf, sizeof sbuf,
// 										 NI_NUMERICHOST | NI_NUMERICSERV);
// 					if (tmpint == 0)
// 					{
// 						printf("Accepted connection on descriptor %d "
// 							   "(host=%s, port=%s)\n",
// 							   infd, hbuf, sbuf);
// 					}

// 					conn_cnt++; // for test clear when close server *****************
// 					printf("conn_cnt:%d\n", conn_cnt);

// 					// test read data in buffer
// 					// int		valread;
// 					// char	buffer[1024];
// 					valread = read(infd, buffer, 1024);
// 					printf("line 344 num read:%d\n", valread);
// 					printf("%s\n", buffer);
// 					// if (valread < 0)
// 					// 	printf("[ERROR]Read Fn error\n");
// 					// else {
// 					// 	printf("[INFO]Read Fn ok\n");
// 					// 	// printf("Read Fn ok %c\n", buffer[0]);
// 					// 	// printf("Read Fn ok\n\tData: %s\n", buffer);
// 					// }

// 					// send(infd, &hello, strlen(hello), 0);
// 					if (event_ret[i].events & EPOLLIN)
// 					{
// 						// if (event_ret[i].events & EPOLLOUT) {
// 						printf("[INFO] Sending Response \n");
// 						send(infd, &responseOkPage, strlen(responseOkPage), 0);
// 						// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 					}

// 					// close(infd); // *************** test for other option

// 					// /* Make the incoming socket non-blocking and add it to the
// 					// 	list of fds to monitor. */
// 					// tmpint = make_socket_non_blocking (infd);
// 					// if (tmpint == -1)
// 					// 	abort ();

// 					event.data.fd = infd;
// 					// event.events = EPOLLIN | EPOLLET;
// 					event.events = EPOLLIN;
// 					// event.events = EPOLLIN | EPOLLET | EPOLLIN | EPOLLOUT;
// 					// event.events = EPOLLIN | EPOLLOUT; // error
// 					tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infd, &event);
// 					if (tmpint == -1)
// 					{
// 						perror("epoll_ctl accept");
// 						abort();
// 					}
// 				}
// 				else
// 				{
// 					/* We have data on the fd waiting to be read. Read and
// 					display it. We must read whatever data is available
// 					completely, as we are running in edge-triggered mode
// 					and won't get a notification again for the same
// 					data. */
// 					if (event_ret[i].events && EPOLLIN)
// 					{
// 						int done = 0;
// 						printf("++++++++ [INFO] Get Data From Client Socket ++++++++\n");

// 						while (1)
// 						{
// 							ssize_t count;
// 							char buf[512];

// 							count = read(event_ret[i].data.fd, buf, sizeof buf);
// 							if (count == -1)
// 							{
// 								/* If errno == EAGAIN, that means we have read all
// 									data. So go back to the main loop. */
// 								printf("Err from read packet\n");
// 								if (errno != EAGAIN)
// 								{
// 									perror("read");
// 									done = 1;
// 								}
// 								break;
// 							}
// 							else if (count == 0)
// 							{
// 								/* End of file. The remote has closed the
// 									connection. */
// 								printf("End read packet\n");
// 								done = 1;
// 								break;
// 							}

// 							/* Write the buffer to standard output */
// 							tmpint = write(1, buf, count);
// 							if (tmpint == -1)
// 							{
// 								perror("write");
// 								abort();
// 							}

// 							// test send data back to socket
// 							printf("\n[INFO] send packet back to client\n");
// 							send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), 0);
// 							// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 						}

// 						if (done)
// 						{
// 							printf("Closed connection on descriptor %d\n", event_ret[i].data.fd);

// 							/* Closing the descriptor will make epoll remove it
// 								from the set of descriptors which are monitored. */
// 							close(event_ret[i].data.fd);
// 						}
// 					}
// 				}
// 			}
// 		}

// 		// for (i = 0; i < n; i++) {
// 		// 	if ((event_ret[i].events & EPOLLERR) || (event_ret[i].events & EPOLLHUP) || (!(event_ret[i].events & EPOLLIN))) {
// 		// 		/* An error has occured on this fd, or the socket is not
// 		// 			ready for reading (why were we notified then?) */
// 		// 		fprintf (stderr, "epoll error\n");
// 		// 		fprintf(stderr, "fd %d err\n", event_ret[i].data.fd);
// 		// 		close (event_ret[i].data.fd);
// 		// 		continue;
// 		// 	}
// 		// 	else if (server_fd0 == event_ret[i].data.fd)
// 		// 	{
// 		// 		/* We have a notification on the listening socket, which
// 		// 		means one or more incoming connections. */
// 		// 		// while (1)
// 		// 		// {
// 		// 			struct sockaddr	in_addr;
// 		// 			socklen_t		in_len;
// 		// 			int				infd;
// 		// 			char			hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

// 		// 			printf("++++++++ [INFO] Serv Socket 1 Recv Data ++++++++\n");
// 		// 			in_len = sizeof in_addr;
// 		// 			infd = accept (server_fd0, &in_addr, &in_len);
// 		// 			// printf("Accept %d fd\n", infd);
// 		// 			if (infd == -1) {
// 		// 				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
// 		// 					/* We have processed all incoming
// 		// 						connections. */
// 		// 					break;
// 		// 				}
// 		// 				else {
// 		// 					perror ("accept");
// 		// 					break;
// 		// 				}
// 		// 			}

// 		// 			tmpint = getnameinfo (&in_addr, in_len,
// 		// 								hbuf, sizeof hbuf,
// 		// 								sbuf, sizeof sbuf,
// 		// 								NI_NUMERICHOST | NI_NUMERICSERV);
// 		// 			if (tmpint == 0) {
// 		// 				printf("Accepted connection on descriptor %d "
// 		// 						"(host=%s, port=%s)\n", infd, hbuf, sbuf);
// 		// 			}

// 		// 			conn_cnt++; // for test clear when close server *****************
// 		// 			printf("conn_cnt:%d\n", conn_cnt);

// 		// 			// test read data in buffer
// 		// 			// int		valread;
// 		// 			// char	buffer[1024];
// 		// 			valread = read(infd, buffer, 1024);
// 		// 			printf("num read:%d\n", valread);
// 		// 			printf("%s\n", buffer);
// 		// 			// if (valread < 0)
// 		// 			// 	printf("Read Fn error\n");
// 		// 			// else {
// 		// 			// 	// printf("Read Fn ok\n");
// 		// 			// 	printf("Read Fn ok %c\n", buffer[0]);
// 		// 			// 	// printf("Read Fn ok\n\tData: %s\n", buffer);
// 		// 			// }

// 		// 			// send(infd, &hello, strlen(hello), 0);
// 		// 			if (event_ret[i].events & EPOLLIN) {
// 		// 			// if (event_ret[i].events & EPOLLOUT) {
// 		// 				printf("[INFO] Sending Response \n");
// 		// 				send(infd, &responseOkPage, strlen(responseOkPage), 0);
// 		// 				// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 		// 			}

// 		// 			// close(infd); // *************** test for other option

// 		// 			// /* Make the incoming socket non-blocking and add it to the
// 		// 			// 	list of fds to monitor. */
// 		// 			// tmpint = make_socket_non_blocking (infd);
// 		// 			// if (tmpint == -1)
// 		// 			// 	abort ();

// 		// 			event.data.fd = infd;
// 		// 			// event.events = EPOLLIN | EPOLLET;
// 		// 			event.events = EPOLLIN;
// 		// 			// event.events = EPOLLIN | EPOLLET | EPOLLIN | EPOLLOUT;
// 		// 			// event.events = EPOLLIN | EPOLLOUT; // error
// 		// 			tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infd, &event);
// 		// 			if (tmpint == -1) {
// 		// 				perror ("epoll_ctl accept");
// 		// 				abort ();
// 		// 			}
// 		// 		// }
// 		// 		continue;
// 		// 	}
// 		// 	else if (server_fd1 == event_ret[i].data.fd)
// 		// 	{
// 		// 		/* We have a notification on the listening socket, which
// 		// 		means one or more incoming connections. */
// 		// 		// while (1)
// 		// 		// {
// 		// 			struct sockaddr	in_addr;
// 		// 			socklen_t		in_len;
// 		// 			int				infd;
// 		// 			char			hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

// 		// 			printf("++++++++ [INFO] Serv Socket 2 Recv Data ++++++++\n");
// 		// 			in_len = sizeof in_addr;
// 		// 			infd = accept (server_fd1, &in_addr, &in_len);
// 		// 			// printf("Accept %d fd\n", infd);
// 		// 			if (infd == -1) {
// 		// 				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
// 		// 					/* We have processed all incoming
// 		// 						connections. */
// 		// 					break;
// 		// 				}
// 		// 				else {
// 		// 					perror ("accept");
// 		// 					break;
// 		// 				}
// 		// 			}

// 		// 			tmpint = getnameinfo (&in_addr, in_len,
// 		// 								hbuf, sizeof hbuf,
// 		// 								sbuf, sizeof sbuf,
// 		// 								NI_NUMERICHOST | NI_NUMERICSERV);
// 		// 			if (tmpint == 0) {
// 		// 				printf("Accepted connection on descriptor %d "
// 		// 						"(host=%s, port=%s)\n", infd, hbuf, sbuf);
// 		// 			}

// 		// 			conn_cnt++; // for test clear when close server *****************
// 		// 			printf("conn_cnt:%d\n", conn_cnt);

// 		// 			// test read data in buffer
// 		// 			// int		valread;
// 		// 			// char	buffer[1024];
// 		// 			valread = read(infd, buffer, 1024);
// 		// 			printf("num read:%d\n", valread);
// 		// 			printf("%s\n", buffer);
// 		// 			// if (valread < 0)
// 		// 			// 	printf("Read Fn error\n");
// 		// 			// else {
// 		// 			// 	// printf("Read Fn ok\n");
// 		// 			// 	printf("Read Fn ok %c\n", buffer[0]);
// 		// 			// 	// printf("Read Fn ok\n\tData: %s\n", buffer);
// 		// 			// }

// 		// 			// send(infd, &hello, strlen(hello), 0);
// 		// 			if (event_ret[i].events & EPOLLIN) {
// 		// 			// if (event_ret[i].events & EPOLLOUT) {
// 		// 				printf("[INFO] Sending Response \n");
// 		// 				send(infd, &responseOkPage, strlen(responseOkPage), 0);
// 		// 				// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 		// 			}

// 		// 			// close(infd); // *************** test for other option

// 		// 			// /* Make the incoming socket non-blocking and add it to the
// 		// 			// 	list of fds to monitor. */
// 		// 			// tmpint = make_socket_non_blocking (infd);
// 		// 			// if (tmpint == -1)
// 		// 			// 	abort ();

// 		// 			event.data.fd = infd;
// 		// 			// event.events = EPOLLIN | EPOLLET;
// 		// 			event.events = EPOLLIN;
// 		// 			// event.events = EPOLLIN | EPOLLET | EPOLLIN | EPOLLOUT;
// 		// 			// event.events = EPOLLIN | EPOLLOUT; // error
// 		// 			tmpint = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infd, &event);
// 		// 			if (tmpint == -1) {
// 		// 				perror ("epoll_ctl accept");
// 		// 				abort ();
// 		// 			}
// 		// 		// }
// 		// 		continue;
// 		// 	}
// 		// 	// receive data from registered socket in poll
// 		// 	else {
// 		// 		/* We have data on the fd waiting to be read. Read and
// 		// 			display it. We must read whatever data is available
// 		// 			completely, as we are running in edge-triggered mode
// 		// 			and won't get a notification again for the same
// 		// 			data. */
// 		// 		if (event_ret[i].events && EPOLLIN) {
// 		// 			int done = 0;
// 		// 			printf("++++++++ [INFO] Get Data From Client Socket ++++++++\n");

// 		// 			while (1)
// 		// 			{
// 		// 				ssize_t	count;
// 		// 				char	buf[512];

// 		// 				count = read (event_ret[i].data.fd, buf, sizeof buf);
// 		// 				if (count == -1) {
// 		// 					/* If errno == EAGAIN, that means we have read all
// 		// 						data. So go back to the main loop. */
// 		// 					printf("Err from read packet\n");
// 		// 					if (errno != EAGAIN) {
// 		// 						perror ("read");
// 		// 						done = 1;
// 		// 					}
// 		// 					break;
// 		// 				}
// 		// 				else if (count == 0) {
// 		// 					/* End of file. The remote has closed the
// 		// 						connection. */
// 		// 					printf("End read packet\n");
// 		// 					done = 1;
// 		// 					break;
// 		// 				}

// 		// 				/* Write the buffer to standard output */
// 		// 				tmpint = write (1, buf, count);
// 		// 				if (tmpint == -1) {
// 		// 					perror ("write");
// 		// 					abort ();
// 		// 				}

// 		// 				// test send data back to socket
// 		// 				printf("\n[INFO] send packet back to client\n");
// 		// 				send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), 0);
// 		// 				// send(event_ret[i].data.fd, &responseOkPage, strlen(responseOkPage), MSG_DONTWAIT);
// 		// 			}

// 		// 			if (done) {
// 		// 				printf ("Closed connection on descriptor %d\n", event_ret[i].data.fd);

// 		// 				/* Closing the descriptor will make epoll remove it
// 		// 					from the set of descriptors which are monitored. */
// 		// 				close (event_ret[i].data.fd);
// 		// 			}
// 		// 		}
// 		// 	}
// 		// }

// 		if (conn_cnt == 4)
// 		{
// 			break;
// 		}
// 	}

// 	free(event_ret);

// 	if (close(server_fd0))
// 	{
// 		fprintf(stderr, "Failed to close server file descriptor\n");
// 		return 1;
// 	}

// 	if (close(server_fd1))
// 	{
// 		fprintf(stderr, "Failed to close server file descriptor\n");
// 		return 1;
// 	}

// 	if (close(epoll_fd))
// 	{
// 		fprintf(stderr, "Failed to close epoll file descriptor\n");
// 		return 1;
// 	}

// 	return 0;
// }

int main(void)
{
	char request[] = "DELETE hTtP:// HTTP/1.1\n"
					 "hOSt: www.example.com\r\n"
					 "Content-type: text/html\r\n"
					 "Content-Length: 25\n\r\r\r\n"
					 "this is body field";
	std::string request2(request);
	Request obj(request2);
	obj.parseRequest();
	return (0);
}
