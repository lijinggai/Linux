.PHONY:all
all:server client

client:client.cc
	g++ -o $@ $^ -std=c++11 -lpthread
server:server.cc
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
	rm -f client server