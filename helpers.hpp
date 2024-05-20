#ifndef HELPERS
#define HELPERS

#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// function that adds a line to the message
void compute_message(char *message, const char *line);

//function that opens a connection to the server
int open_connection();

// function that closes the connection to the server
void close_connection_to_server(int sockfd);

#endif