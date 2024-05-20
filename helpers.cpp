#include "helpers.hpp"

// function that adds a line to the message
void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

//function that opens a connection to the server
int open_connection_to_server()
{
    int sockfd;
    struct sockaddr_in serv_addr;

    // open the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("error opening the socket");
        exit(0);
    }

    // set the server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = inet_addr("34.246.184.49");

    // connect to the server
    if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("error connecting to the server");
        exit(0);
    }
    
    // retunr the socket
    return sockfd;
}

// function that closes the connection to the server
void close_connection_to_server(int sockfd)
{
    close(sockfd);
}