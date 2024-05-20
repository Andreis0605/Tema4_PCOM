all: client

client: client.o requests.o
	g++ client.o requests.o -o client -Wall

client.o: client.cpp
	g++ -c client.cpp -Wall

requests.o: requests.o
	g++ -c requests.cpp -Wall

clean:
	rm -f *.o client