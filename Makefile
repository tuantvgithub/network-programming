all: server client

server : 
	gcc ./code/server-side/server.c ./code/server-side/postman.c ./code/server-side/storage.c -o server 
client : 
	gcc ./code/client-side/client.c ./code/client-side/client-ui.c ./code/client-side/validate.c -o server 
