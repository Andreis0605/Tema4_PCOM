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

### Flow-ul clientului

> Clientul asteapta input de la utilizator. Pentru orice actiune (mai putin exit, la care pur si simplu se inchide) acesta deschide o conexiune cu server-ul, trimite un mesaj, asteapta un raspuns si inchide conexiunea. Apoi proceseaza raspunsul primit de la server.