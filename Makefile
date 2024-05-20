all: client

client: client.o requests.o helpers.o
	g++ client.o requests.o helpers.o -o client -Wall

client.o: client.cpp
	g++ -c client.cpp -Wall

requests.o: requests.o
	g++ -c requests.cpp -Wall

helpers.o: helpers.cpp
	g++ -c helpers.cpp -Wall

clean:
	rm -f *.o client