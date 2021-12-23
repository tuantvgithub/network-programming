all:  client server

client : client.o validate.o client-ui.o postman.o
	gcc client.o validate.o client-ui.o postman.o -o client 
server : server.o storage.o postman.o 
	gcc server.o storage.o postman.o -o server

client.o : ./client-side/client.c
	gcc -c ./client-side/client.c
client-ui.o : ./client-side/client-ui.c
	gcc -c ./client-side/client-ui.c
validate.o : ./client-side/validate.c
	gcc -c ./client-side/validate.c
postman.o : ./server-side/postman.c
	gcc -c ./server-side/postman.c
storage.o : ./server-side/storage.c
	gcc -c ./server-side/storage.c
server.o : ./server-side/server.c
	gcc -c ./server-side/server.c

clean :
	rm *.o