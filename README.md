# Implementarea temei pleaca de la scheletul laboratorului 9

# Tema 4 PCOM: REST API Client

## Fisiere luate din laborator

> Am luat fisierul "buffer.c" si "buffer.h" si le-am schimbat in "buffer.cpp" si "buffer.hpp", facand modificarile de rigoare pentru a putea fi compilate in C++.

> Am luat fisierul "helper.c" si "helper.h" si le-am schimbat in "helper.cpp" si "helper.hpp", facand modificarile de rigoare pentru a putea fi compilate in C++. In plus, am modificat functia functia `open_connection_to_server` pentru a deschide un socket direct catre serverul temei si nu a mai fi nevoie de parametrii.

## Fisierul requests.cpp

> Implementarile functilor din acest fisier pleaca de la indicatiile din scheletul de laborator.

- `char *compute_get_request(char *host, char *url, char *cookies, char *token)`

    >  Functia genereaza un mesaj pentru un GET request care va fi trimis catre server. Aceasta construieste mesajul prin adaugarea pe rand a campurilor pentru un mesaj http: tipul de request, url-ul catre care va fi trimis, host-ul, si, daca exista, cookie-ul de sesiune si token-ul jwt. Apoi returneaza mesajul.

- `char *compute_post_request(char *host, char *url, char *content_type, const char *body_data, char *cookies, char *token)`

    > Functia genereaza un mesaj pentru un POST request care va fi trimis catre server. Aceasta construieste mesajul prin adaugarea pe rand a campurilor pentru acest tip de mesaj http: tipul de request, url-ul catre care va fi trimis, host-ul, content type-ul, content length-ul, cookie-ul de sesiune si token-ul jwt, daca exista, si continutul propriu-zis. Apoi returneaza mesajul.

- `char *compute_delete_request(char *host, char *url, char *cookies, char *token)`

    > Functie similara cu cea de GET request. Aceasta construieste mesajul prin adaugarea pe rand a campurilor pentru un mesaj http: tipul de request, url-ul catre care va fi trimis, host-ul, si, daca exista, cookie-ul de sesiune si token-ul jwt. Apoi returneaza mesajul.

## Clientul

### Functii definite in client

- `char *get_cookie(char *response)`

    > Functia extrage un cookie din raspusul primit de la server in momentul in care un utilizator se logheaza. Returneaza cookie-ul extras.

- `int get_error_code(char *response)`

    > Functie care returneaza codul din raspunsul primit de la server.

- `json get_json_response(char *response)`

    > Funtie care extrage un json din raspunsul primit de la server. Intoarce un obiect json obtinut prin parsarea mesajului sau un obiect json vid in caz ca nu exista un json in raspunsul serverului.

### Biblioteca de JSON folosita

> Am ales sa folosesc biblioteca nlohmann pentru lucrul cu JSON, deoarece a fost recomandata in cerinta si datorita faptului ca este usor de inclus in program (un singur fisier: "json.hpp").

### Flow-ul clientului

> Clientul asteapta input de la utilizator. Pentru orice actiune (mai putin exit, la care pur si simplu se inchide) acesta deschide o conexiune cu server-ul, trimite un mesaj, asteapta un raspuns si inchide conexiunea. Apoi proceseaza raspunsul primit de la server.

### Functionalitati implementate in client

- #### Register

    > Pentru aceasta functionalitate citesc datele introduse de utilizator si verific sa fie valide. Apoi construiesc un obiect json care contine username-ul si parola. Dupa aceea, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. La final, verific codul returnat de server.

- #### Login

    > Pentru aceasta functionalitate verific daca nu am un utilizator deja logat. Daca am, printez eroare. Daca nu, citesc datele de la tastatura si verific sa fie valide. Apoi construiesc un obiect json cu username-ul si parola. In continuare, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. La final, verific codul returnat de server. Daca am un cod bun, extrag cookie-ul de sesiune din mesaj.

- #### Enter_library

    > Pentru acesasta functionalitate verific daca am un user logat. Daca nu, printez eroare. Daca da, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. In continuare, verific codul returnat de server. Daca am un cod bun, extrag token-ul jwt din mesaj.

- #### Get_books

    > Pentru aceasta functionalitate verifc daca am cookie-ul de sesiune si token-ul jwt. Daca nu, printez eroare. Daca da, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. La final, verific codul returntat de server. Daca am un cod bun, extrag JSON-ul din raspuns si il afisez.

- #### Get_book

    > Pentru aceasta functionalitate verifc daca am cookie-ul de sesiune si token-ul jwt. Daca nu, printez eroare. Daca da, citesc datele introduse de utilizator si verific sa fie valide. In continuare, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. Apoi verific codul returnat de server.  Daca am un cod bun, extrag JSON-ul din raspuns si il afisez.

- #### Add_book

    > Pentru aceasta functionalitate verifc daca am cookie-ul de sesiune si token-ul jwt. Daca nu, printez eroare. Daca da, citesc datele introduse de utilizator si verific sa fie valide. In continuare, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. Apoi verific codul returnat de server.

- #### Delete_book

    > Pentru aceasta functionalitate verifc daca am cookie-ul de sesiune si token-ul jwt. Daca nu, printez eroare. Daca da, citesc datele introduse de utilizator si verific sa fie valide. In continuare, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. Apoi verific codul returnat de server.

- #### Logout

    > Pentru aceasta functionalitate verifc daca am cookie-ul de sesiune si token-ul jwt. Daca nu, printez eroare. Daca da, deschid o conexiune catre server, construiesc mesajul, il trimit, primesc raspunsul si inchid conexiunea. La final, verific codul returntat de server. Daca am un cod bun, sterg cookie-ul si token-ul.

- #### Exit

    > Pentru aceasta functionalitate sterg cookie-ul si token-ul pe care il am si opresc bucla in care citest si interpretez comenzi.


