.PHONY: build run clean

run: client
	./client

build: client

client: client.o requests.o helpers.o buffer.o
	g++ client.o requests.o helpers.o buffer.o -o client -Wall

client.o: client.cpp
	g++ -c client.cpp -Wall

requests.o: requests.cpp
	g++ -c requests.cpp -Wall

helpers.o: helpers.cpp
	g++ -c helpers.cpp -Wall

buffer.o: buffer.cpp
	g++ -c buffer.cpp -Wall

clean:
	rm -f *.o client