#ifndef HELPERS
#define HELPERS

#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 4096
#define LINELEN 1000

// function that adds a line to the message
void compute_message(char *message, const char *line);

//function that opens a connection to the server
int open_connection_to_server();

// function that closes the connection to the server
void close_connection_to_server(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);
#endif