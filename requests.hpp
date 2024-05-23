#ifndef _REQUESTS_
#define _REQUESTS_

// function that computes and returns a GET request string
char *compute_get_request(char *host, char *url,
                          char *cookies, char *token);

// function that computes and returns a POST request string
char *compute_post_request(char *host, char *url, char *content_type, const char *body_data,
                           char *cookies, char *token);

// function that computes and returns a DELETE request string
char *compute_delete_request(char *host, char *url,
                             char *cookies, char *token);

#endif