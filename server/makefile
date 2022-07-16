.PHONY:all
all:server client
server:server.cc
		g++ server.cc -o server -g

client:client.cc
		g++ client.cc -o client -g

.PHONY:clean
clean:
		rm -f server client  