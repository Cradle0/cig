#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include "socket.h"  
#include "message.h"
#include "controller.h"

#define MAXFD(x,y) ((x) >= (y)) ? (x) : (y)


int main(int argc, char *argv[]){
	int port;
	struct cignal cig;
	// A buffer to store a serialized message
	char *cig_serialized = malloc(sizeof(char)*CIGLEN);
	char *buffer = malloc(sizeof(char)*30);
	// An array to registered sensor devices
	int device_record[MAXDEV] = {0};
	int device_fd[MAXDEV] = {0};
	
	if (argc == 2) {
		port = strtol(argv[1], NULL, 0);
	} else {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	int gatewayfd = set_up_server_socket(port);
	printf("\nThe Cignal Gateway is now started on port: %d\n\n", port);

	int max_fd = gatewayfd;
	fd_set all_fds;
	FD_ZERO(&all_fds);
	FD_SET(gatewayfd, &all_fds);

	/* TODO: Implement the body of the server.  
	 *
	 * Use select so that the server process never blocks on any call except
	 * select. If no sensors connect and/or send messsages in a timespan of
	 * 5 seconds then select will return and print the message "Waiting for
	 * Sensors update..." and go back to waiting for an event.
	 * 
	 * The server will handle connections from devices, will read a message from
	 * a sensor, process the message (using process_message), write back
	 * a response message to the sensor client, and close the connection.
	 * After reading a message, your program must print the "RAW MESSAGE"
	 * message below, which shows the serialized message received from the *
	 * client.
	 * 
	 *  Print statements you must use:
     * 	printf("Waiting for Sensors update...\n");
	 * 	printf("RAW MESSAGE: %s\n", YOUR_VARIABLE);
	 */

	// TODO implement select loop
	while (1){
		int i = 0;
		fd_set listen_fds = all_fds;

		// Set alarm
		struct timeval alarm;
		alarm.tv_sec = 5;
		alarm.tv_usec = 0;

		int result = select(max_fd + 1, &listen_fds, NULL, NULL, &alarm);
		if (result == -1){
			fprintf(stderr, "Gateway select failed\n");
			exit(1);
		} else if (result == 0){
			printf("Waiting for Sensors update...\n");
		}

		// Check for connections? Accept them THIS JUSTS ACCEPTS CONNECTIONS
		if (FD_ISSET(gatewayfd, &listen_fds)){
			int client_fd = accept_connection(gatewayfd);
			if (client_fd == -1){
				fprintf(stderr, "Connection error\n");
				exit(1);
			} 

			device_fd[i] = client_fd;
			i++;

			// How select works lol
			if (client_fd > max_fd){
				max_fd = client_fd;
			}
			FD_SET(client_fd, &all_fds);
		}

		// Check clients and see what they are saying
		// I'm positive MAXDEV can be replaced by result. But I'm not confident
		for (int index = 0; index < MAXDEV; index++){
			if (FD_ISSET(device_fd[index], &listen_fds)){
				int num_read = read(device_fd[index], buffer, 30);
				if (num_read == 0){
					FD_CLR(device_fd[index], &all_fds);
				}
				else if (num_read == -1){
					fprintf(stderr, "Read error\n");
					exit(1);
				}
				else {
					printf("RAW MESSAGE: %s\n", buffer);
					unpack_cignal(buffer, &cig);
					if (process_message(&cig, device_record) == -1){
						fprintf(stderr, "Process message failed, closing connection\n");
						FD_CLR(device_fd[index], &all_fds);
					}
					if (cig.hdr.type == UPDATE) {
						adjust_fan(&cig);
					}
					cig_serialized = serialize_cignal(cig);
					int num_write = write(device_fd[index], cig_serialized, 20);
					if (num_write == -1 || num_write != CIGLEN){
						fprintf(stderr, "Write error\n");
						exit(1);
					}
				}
			}
		}
	}
	return 0;
}
