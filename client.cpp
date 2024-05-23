#include <bits/stdc++.h>

#include "json.hpp"
#include "helpers.hpp"
#include "requests.hpp"
#include "buffer.hpp"

// todo: check if the input is valid

using namespace std;
using json = nlohmann::json;

char *get_cookie(char *response)
{
    char *cookie = strstr(response, "Set-Cookie: ");
    cookie += 12;
    char *end_cookie = strchr(cookie, ';');
    *end_cookie = '\0';
    return cookie;
}

int get_error_code(char *response)
{
    char *error_code = strchr(response, ' ');
    return atoi(error_code + 1);
}

json get_json_response(char *response)
{
    char *json_start = basic_extract_json_response(response);
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

    // buffer used for reading from stdin
    char stdin_buffer[200];
    char *jwt = NULL;
    char *cookie = NULL;
    char *host = (char *)calloc(500, sizeof(char));
    strcpy(host, "34.246.184.49:8080");
    char *content_type = (char *)calloc(500, sizeof(char));
    strcpy(content_type, "application/json");

    while (true)
    {
        // read the input from stdin
        cin >> stdin_buffer;

        // if the input is exit, close the program
        if (strstr(stdin_buffer, "exit"))
        {
            break;
        }

        // process the register user request
        if (strstr(stdin_buffer, "register"))
        {
            cin.get();

            // read the data from the input
            char username[500], password[500];
            cout << "username=";
            cin.getline(username, 500);
            cout << "password=";
            cin.getline(password, 500);

            /*cout << username << '\n'
                 << password << '\n';*/

            // check if the input is valid
            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "ERROR: spaces not allowed in username or password" << '\n';
                continue;
            }

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/register");

            char *message = compute_post_request(host, url, content_type, j.dump().c_str(), NULL, 0);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            int nr = get_error_code(response);
            json response_json = get_json_response(response);

            if (nr / 100 == 2)
            {
                cout << "SUCCES: user registered" << '\n';
            }
            else
            {
                cout << "ERROR: user already exists" << '\n';
            }

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

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/login");

            char *message =
                compute_post_request(host,
                                     url,
                                     content_type,
                                     j.dump().c_str(), NULL, NULL);

            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);
            int nr = get_error_code(response);

            if (nr / 100 == 2)
            {
                cout << "SUCCES: user logged in" << '\n';
                json response_json = get_json_response(response);
                cookie = get_cookie(response);
            }
            else
            {
                cout << "ERROR: invalid username or password" << '\n';
            }
            free(url);
        }

        if (strstr(stdin_buffer, "enter_library"))
        {
            if (cookie == NULL)
            {
                cout << "ERROR: Must be logged in order to acces the library";
                cout << '\n';
                continue;
            }

            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/access");
            char *message =
                compute_get_request(host,
                                    url, NULL,
                                    cookie, jwt);

            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            int nr = get_error_code(response);
            json response_json = get_json_response(response);
            string jwt_token = response_json["token"].get<string>();
            jwt = new char[jwt_token.length() + 1];
            strcpy(jwt, jwt_token.c_str());

            if (nr / 100 == 2)
            {
                cout << "SUCCES: acces granted to the library" << '\n';
            }
            else
            {
                cout << "ERROR: acces denied to the library" << '\n';
            }
            free(url);
        }

        if (strstr(stdin_buffer, "get_books"))
        {
            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/books");

            char *message =
                compute_get_request(host,
                                    url, NULL,
                                    cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            char *json_end = strchr(response, ']');
            *(json_end + 1) = '\0';

            json response_json = json::parse(strchr(response, '['));

            cout << response_json.dump(4) << '\n';

            free(url);

            continue;
        }

        if (strstr(stdin_buffer, "get_book"))
        {
            char id[200];
            cout << "id=";
            cin >> id;

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

            int sockfd = open_connection_to_server();

            char *book = (char *)malloc(500);
            strcpy(book, "/api/v1/tema/library/books/");
            strcat(book, id);

            char *message =
                compute_get_request(host, book, NULL, cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            json response_json = get_json_response(response);

            cout << response_json.dump(4) << '\n';
            free(book);
        }

        if (strstr(stdin_buffer, "add_book"))
        {
            cin.get();
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

            json j;
            j = {
                {"title", title},
                {"author", author},
                {"genre", genre},
                {"publisher", publisher},
                {"page_count", atoi(page_count)}};

            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/library/books");

            char *message = compute_post_request(host, url, content_type, j.dump().c_str(), cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            // cout << response;

            int nr = get_error_code(response);
            if (nr / 100 == 2)
            {
                cout << "SUCCES: book added to the library" << '\n';
            }
            else
            {
                cout << "ERROR: could not add book to the library" << '\n';
            }
            free(url);
        }

        if (strstr(stdin_buffer, "delete_book"))
        {
            char id[500];
            cout << "id=";
            cin >> id;

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

            int sockfd = open_connection_to_server();

            char *book = (char *)malloc(500);
            strcpy(book, "/api/v1/tema/library/books/");
            strcat(book, id);

            char *message =
                compute_delete_request(host, book, NULL, cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            int nr = get_error_code(response);

            if (nr / 100 == 2)
            {
                cout << "SUCCES: book deleted from the library" << '\n';
            }
            else
            {
                cout << "ERROR: could not delete book from the library" << '\n';
            }
        }

        if (strstr(stdin_buffer, "logout"))
        {
            if (cookie == NULL)
            {
                cout << "ERROR: no user logged in" << '\n';
                continue;
            }

            int sockfd = open_connection_to_server();

            char *url = (char *)malloc(500);
            strcpy(url, "/api/v1/tema/auth/logout");

            char *message =
                compute_get_request(host, url, NULL, cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            int nr = get_error_code(response);
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
            free(url);
        }
    }

    free(host);
    return 0;
}