all:  client server

client : client.o validate.o client-ui.o postman.o
	gcc client.o validate.o client-ui.o postman.o -o client 
server : server.o storage.o postman.o 
	gcc server.o storage.o postman.o -o server

client.o : client.c
	gcc -c client.c
client-ui.o : client-ui.c
	gcc -c client-ui.c
validate.o : validate.c
	gcc -c validate.c
postman.o : postman.c
	gcc -c postman.c
storage.o : storage.c
	gcc -c storage.c
server.o : server.c
	gcc -c server.c

clean :
	rm *.o