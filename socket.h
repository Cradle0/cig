#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <netinet/in.h>    /* Internet domain header, for struct sockaddr_in */

#define QUEUE_SIZE 5 // Size of the listen queue

int set_up_server_socket(int port);
int accept_connection(int listenfd);

int connect_to_server(int port, const char *hostname);

#endif
