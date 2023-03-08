.PHONY:all
all:server client

client:client.cc
	g++ -o $@ $^
server:server.cc
	g++ -o $@ $^
.PHONY:clean
	rm -f client server