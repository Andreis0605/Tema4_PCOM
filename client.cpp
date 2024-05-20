#include <bits/stdc++.h>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main()
{

    // buffer used for reading from stdin
    char stdin_buffer[200];
    char jwt[500] = {0};

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

            //char *message = compute_post_request();
        }
    }
    return 0;
}