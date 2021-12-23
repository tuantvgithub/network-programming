all: server client

server : server.c postman.c
	gcc server.c postman.c -o server
client : client.c postman.c client-ui.c validate.c
	gcc client.c postman.c client-ui.c validate.c -o client
clean:
	rm client-ui
