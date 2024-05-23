#include "helpers.hpp"
#include "buffer.hpp"

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// function that adds a line to the message
void compute_message(char *message, const char *line)
{
    strcat(message, line);
    strcat(message, "\r\n");
}

// modified this function from the lab in order to
// have the the server address and port hardcoded
// function that opens a connection to the server
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
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("error connecting to the server");
        exit(0);
    }

    // retunr the socket
    return sockfd;
}

void close_connection_to_server(int sockfd)
{
    close(sockfd);
}

void send_to_server(int sockfd, char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
        {
            error("ERROR writing message to socket");
        }

        if (bytes == 0)
        {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

char *receive_from_server(int sockfd)
{
    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    do
    {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0)
        {
            error("ERROR reading response from socket");
        }

        if (bytes == 0)
        {
            break;
        }

        buffer_add(&buffer, response, (size_t)bytes);

        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0)
        {
            header_end += HEADER_TERMINATOR_SIZE;

            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);

            if (content_length_start < 0)
            {
                continue;
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);
    size_t total = content_length + (size_t)header_end;

    while (buffer.size < total)
    {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0)
        {
            error("ERROR reading response from socket");
        }

        if (bytes == 0)
        {
            break;
        }

        buffer_add(&buffer, response, (size_t)bytes);
    }
    buffer_add(&buffer, "", 1);
    return buffer.data;
}

char *basic_extract_json_response(char *str)
{
    return strstr(str, "{\"");
}