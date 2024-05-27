CXX = g++

all: server client

server : server.cpp
	$(CXX) -o server server.cpp

client : client.cpp
	$(CXX) -o client client.cpp

clean:
	rm client server

.PHONY: all clean