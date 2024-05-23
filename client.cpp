#include <bits/stdc++.h>

#include "json.hpp"
#include "helpers.hpp"
#include "requests.hpp"
#include "buffer.hpp"

using namespace std;
using json = nlohmann::json;

// function that extracts the cookie from the login response
char *get_cookie(char *response)
{
    // find the begining of the cookie
    char *cookie = strstr(response, "Set-Cookie: ");
    cookie += 12;

    // find the end of the cookie
    char *end_cookie = strchr(cookie, ';');
    *end_cookie = '\0';

    // return the cookie
    return cookie;
}

// function that extracts the error code from a response
int get_error_code(char *response)
{
    // search for the first space in the response
    char *error_code = strchr(response, ' ');

    // get the number
    return atoi(error_code + 1);
}

// function that extracts the JSON part from a response
json get_json_response(char *response)
{
    // call the function that extracts the JSON part
    char *json_start = basic_extract_json_response(response);

    // check if the JSON part is found
    if (json_start)
    {
        // Convert the JSON part to a JSON object
        return json::parse(json_start);
    }
    else
    {
        // Return an empty JSON object if no JSON is found
        return json::object();
    }
}

int main()
{

    // declare the buffers used
    char stdin_buffer[200];                         // for stdin input
    char *jwt = NULL;                               // for the jwt token
    char *cookie = NULL;                            // for the cookie
    char *host = (char *)calloc(500, sizeof(char)); // for the host
    strcpy(host, "34.246.184.49:8080");
    char *content_type =
        (char *)calloc(500, sizeof(char)); // for the content type
    strcpy(content_type, "application/json");

    while (true)
    {
        // read the input from stdin
        cin >> stdin_buffer;

        // if the input is exit, close the program
        if (strstr(stdin_buffer, "exit"))
        {
            cookie = NULL;
            jwt = NULL;
            break;
        }

        // process the register user request
        if (strstr(stdin_buffer, "register"))
        {

            // check if a user is not already connected
            if (cookie != NULL || jwt != NULL)
            {
                cout << "ERROR: A user is already logged in" << '\n';
                continue;
            }

            // get the newline character left in the buffer
            cin.get();

            // read the data from the input
            char username[500], password[500];
            cout << "username=";
            cin.getline(username, 500);
            cout << "password=";
            cin.getline(password, 500);

            // check if the input is valid
            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "ERROR: spaces not allowed in username or password" << '\n';
                continue;
            }
            if (strlen(username) == 0 || strlen(password) == 0)
            {
                cout << "ERROR: username and password can not be empty" << '\n';
                continue;
            }

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/register");
            char *message = compute_post_request(host, url, content_type, j.dump().c_str(), NULL, 0);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);

            // process the error code
            if (nr / 100 == 2)
            {
                cout << "SUCCES: user registered" << '\n';
            }
            else
            {
                cout << "ERROR: user already exists" << '\n';
            }

            // free the url
            free(url);
        }

        // process the login user request
        if (strstr(stdin_buffer, "login"))
        {
            // check if the user is not already connected
            if (cookie != NULL || jwt != NULL)
            {
                cout << "ERROR: User already logged in" << '\n';
                continue;
            }

            // get the newline character left in the buffer
            cin.get();

            // read the data from the input
            char username[500], password[500];
            cout << "username=";
            cin.getline(username, 500);
            cout << "password=";
            cin.getline(password, 500);

            // check if the input is valid
            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "ERROR: spaces not allowed in username or password" << '\n';
                continue;
            }
            if (strlen(username) == 0 || strlen(password) == 0)
            {
                cout << "ERROR: username and password can not be empty" << '\n';
                continue;
            }

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/login");
            char *message =
                compute_post_request(host,
                                     url,
                                     content_type,
                                     j.dump().c_str(), NULL, NULL);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);

            // process the error code
            if (nr / 100 == 2)
            {
                cout << "SUCCES: user logged in" << '\n';

                // get the cookie from the messages
                json response_json = get_json_response(response);
                cookie = get_cookie(response);
            }
            else
            {
                cout << "ERROR: invalid username or password" << '\n';
            }

            // free the url
            free(url);
        }

        // process the enter library request
        if (strstr(stdin_buffer, "enter_library"))
        {
            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to acces the library";
                cout << '\n';
                continue;
            }

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/access");
            char *message =
                compute_get_request(host, url, cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);

            // process the error code
            if (nr / 100 == 2)
            {
                cout << "SUCCES: acces granted to the library" << '\n';

                // get the jwt token from the response
                json response_json = get_json_response(response);
                string jwt_token = response_json["token"].get<string>();
                jwt = new char[jwt_token.length() + 1];
                strcpy(jwt, jwt_token.c_str());
            }
            else
            {
                cout << "ERROR: acces denied to the library" << '\n';
            }

            // free the url
            free(url);
        }

        // process the get books request
        if (strstr(stdin_buffer, "get_books"))
        {
            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to get the books";
                cout << '\n';
                continue;
            }
            if (jwt == NULL)
            {
                cout << "ERROR: Must enter the libray first" << '\n';
                continue;
            }

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/books");
            char *message =
                compute_get_request(host, url, cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the JSON part from the response
            char *json_end = strchr(response, ']');
            *(json_end + 1) = '\0';
            json response_json = json::parse(strchr(response, '['));

            // print the JSON part
            cout << response_json.dump(4) << '\n';

            // free the url
            free(url);

            // go to the next iteration
            // must be used because the next command
            // is matched by this one
            continue;
        }

        if (strstr(stdin_buffer, "get_book"))
        {

            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to get the books";
                cout << '\n';
                continue;
            }
            if (jwt == NULL)
            {
                cout << "ERROR: Must enter the libray first" << '\n';
                continue;
            }

            // get the newline character left in the buffer
            cin.get();

            // read the data from the input
            char id[500];
            cout << "id=";
            cin.getline(id, 500);

            // check if the input is vali
            int id_len = strlen(id);
            bool valid = true;
            for (int i = 0; i < id_len && valid; i++)
            {
                if (id[i] < '0' || id[i] > '9' || id[i] == ' ')
                {
                    cout << "ERROR: id must contain only numbers" << '\n';
                    valid = false;
                }
            }
            if (!valid)
                continue;

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *book = (char *)malloc(500);
            strcpy(book, "/api/v1/tema/library/books/");
            strcat(book, id);
            char *message =
                compute_get_request(host, book, cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the JSON part from the response
            json response_json = get_json_response(response);

            // print the JSON part
            cout << response_json.dump(4) << '\n';

            // free the url
            free(book);
        }

        if (strstr(stdin_buffer, "add_book"))
        {
            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to get the books";
                cout << '\n';
                continue;
            }
            if (jwt == NULL)
            {
                cout << "ERROR: Must enter the libray first" << '\n';
                continue;
            }

            // get the newline character left in the buffer
            cin.get();

            // read the data from the input
            char title[500], author[500], genre[500], publisher[500], page_count[500];
            cout << "title=";
            cin.getline(title, 500);
            cout << "author=";
            cin.getline(author, 500);
            cout << "genre=";
            cin.getline(genre, 500);
            cout << "publisher=";
            cin.getline(publisher, 500);
            cout << "page_count=";
            cin.getline(page_count, 500);

            // check if the input is valid
            int page_count_len = strlen(page_count);
            bool valid = true;
            for (int i = 0; i < page_count_len && valid; i++)
            {
                if (page_count[i] < '0' || page_count[i] > '9' || page_count[i] == ' ')
                {
                    cout << "ERROR: page_count must contain only numbers" << '\n';
                    valid = false;
                }
            }
            if (strlen(title) == 0 ||
                strlen(author) == 0 ||
                strlen(genre) == 0 ||
                strlen(publisher) == 0 ||
                strlen(page_count) == 0)
            {
                cout << "ERROR: All fields must contain information" << '\n';
                valid = false;
            }
            if (!valid)
                continue;

            // create the JSON object for the request
            json j;
            j = {
                {"title", title},
                {"author", author},
                {"genre", genre},
                {"publisher", publisher},
                {"page_count", atoi(page_count)}};

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/books");
            char *message = compute_post_request(host, url, content_type, j.dump().c_str(), cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);
            if (nr / 100 == 2)
            {
                cout << "SUCCES: book added to the library" << '\n';
            }
            else
            {
                cout << "ERROR: could not add book to the library" << '\n';
            }

            // free the url
            free(url);
        }

        // process the delete book request
        if (strstr(stdin_buffer, "delete_book"))
        {
            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to get the books";
                cout << '\n';
                continue;
            }
            if (jwt == NULL)
            {
                cout << "ERROR: Must enter the libray first" << '\n';
                continue;
            }

            // get the newline character left in the buffer
            cin.get();

            char id[500];
            cout << "id=";
            cin.getline(id, 500);

            // check if the input is vali
            int id_len = strlen(id);
            bool valid = true;
            for (int i = 0; i < id_len && valid; i++)
            {
                if (id[i] < '0' || id[i] > '9' || id[i] == ' ')
                {
                    cout << "id must contain only numbers" << '\n';
                    valid = false;
                }
            }
            if (!valid)
                continue;

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *book = (char *)malloc(500);
            strcpy(book, "/api/v1/tema/library/books/");
            strcat(book, id);
            char *message =
                compute_delete_request(host, book, cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);

            // process the error code
            if (nr / 100 == 2)
            {
                cout << "SUCCES: book deleted from the library" << '\n';
            }
            else
            {
                cout << "ERROR: could not delete book from the library" << '\n';
            }

            // free the book
            free(book);
        }

        // process the logout request
        if (strstr(stdin_buffer, "logout"))
        {
            // check if the user is not logged in
            if (cookie == NULL)
            {
                cout << "ERROR: no user logged in" << '\n';
                continue;
            }
            if (jwt == NULL)
            {
                cout << "ERROR: no user logged in" << '\n';
                continue;
            }

            // open the connection to the server
            int sockfd = open_connection_to_server();

            // build the message
            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/logout");
            char *message =
                compute_get_request(host, url, cookie, jwt);

            // send the message to the server
            send_to_server(sockfd, message);

            // get the response from the server
            char *response = receive_from_server(sockfd);

            // close the connection to the server
            close_connection_to_server(sockfd);

            // get the error code from the response
            int nr = get_error_code(response);

            // process the error code
            if (nr / 100 == 2)
            {
                cout << "SUCCES: user logged out" << '\n';
                cookie = NULL;
                jwt = NULL;
            }
            else
            {
                cout << "ERROR: could not log user out" << '\n';
            }

            // free the url
            free(url);
        }
    }

    // free the memory for  host and content type
    free(content_type);
    free(host);
    return 0;
}