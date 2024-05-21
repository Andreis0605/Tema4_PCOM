#include <bits/stdc++.h>

#include "json.hpp"
#include "helpers.hpp"
#include "requests.hpp"
#include "buffer.hpp"

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
            // read the data from the input
            char username[200], password[200];
            cout << "username=";
            cin >> username;
            cout << "password=";
            cin >> password;

            // check if the input is valid
            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "Nu se pot folosi spatii in username sau parola" << '\n';
                continue;
            }

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            int sockfd = open_connection_to_server();
            char *message = compute_post_request("34.246.184.49:8080", "/api/v1/tema/auth/register", "application/json", j.dump().c_str(), NULL, 0);
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
        }

        // process the login user request
        if (strstr(stdin_buffer, "login"))
        {
            // check if the user is not already connected
            if (cookie != NULL || jwt != NULL)
            {
                cout << "User already logged in\n";
                continue;
            }

            // read the data from the input
            char username[200], password[200];
            cout << "username=";
            cin >> username;
            cout << "password=";
            cin >> password;

            // check if the input is valid
            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "Nu se pot folosi spatii in username sau parola" << '\n';
                continue;
            }

            // create the json object for the request
            json j;
            j = {
                {"username", username},
                {"password", password}};

            int sockfd = open_connection_to_server();
            char *message =
                compute_post_request("34.246.184.49:8080",
                                     "/api/v1/tema/auth/login",
                                     "application/json",
                                     j.dump().c_str(), NULL, 0);

            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);
            int nr = get_error_code(response);
            json response_json = get_json_response(response);
            cookie = get_cookie(response);

            if (nr / 100 == 2)
            {
                cout << "SUCCES: user logged in" << '\n';
            }
            else
            {
                cout << "ERROR: invalid username or password" << '\n';
            }
        }

        if (strstr(stdin_buffer, "enter_library"))
        {
            int sockfd = open_connection_to_server();
            char *message =
                compute_get_request("34.246.184.49:8080",
                                    "/api/v1/tema/library/access", NULL,
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
        }

        if (strstr(stdin_buffer, "get_books"))
        {
            int sockfd = open_connection_to_server();
            char *message =
                compute_get_request("34.246.184.49:8080",
                                    "/api/v1/tema/library/books", NULL,
                                    cookie, jwt);
            send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            close_connection_to_server(sockfd);

            char *json_end = strchr(response, ']');
            *(json_end + 1) = '\0';

            // cout << response << '\n';

            int nr = get_error_code(response);
            json response_json = json::parse(strchr(response, '['));

            cout << response_json << '\n';
        }

        
    }
    return 0;
}