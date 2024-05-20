#include <bits/stdc++.h>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main()
{

    //buffer used for reading from stdin
    char stdin_buffer[200];
    char jwt[500]={0};

    while (true)
    {
        cin >> stdin_buffer;

        if (strstr(stdin_buffer, "exit"))
        {
            break;
        }

        if (strstr(stdin_buffer, "register"))
        {
            char username[200], password[200];
            cout << "username=";
            cin >> username;
            cout << "password=";
            cin >> password;

            if (strchr(username, ' ') || strchr(password, ' '))
            {
                cout << "Nu se pot folosi spatii in username sau parola" << '\n';
                continue;
            }

            json j;
            j = {
                {"username", username},
                {"password", password}};
            
            char *message = compute_post_request();
        }
    }
    return 0;
}