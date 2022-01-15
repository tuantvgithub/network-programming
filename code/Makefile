CC = gcc
FLAGS = -Wall -g -c

OUT = out
EXEC = exec

CLIENT_SIDE = client
SERVER_SIDE = server
UTILS_DIR = util

all:  $(EXEC)/client $(EXEC)/server

$(EXEC)/client : $(OUT)/client.o $(OUT)/validate.o $(OUT)/client-ui.o $(OUT)/postman.o $(OUT)/utils.o
	$(CC) $(OUT)/client.o $(OUT)/validate.o $(OUT)/client-ui.o $(OUT)/postman.o $(OUT)/utils.o -o $(EXEC)/client
$(EXEC)/server : $(OUT)/server.o $(OUT)/storage.o $(OUT)/postman.o $(OUT)/utils.o
	$(CC) $(OUT)/server.o $(OUT)/storage.o $(OUT)/postman.o $(OUT)/utils.o -o $(EXEC)/server

$(OUT)/client.o : $(CLIENT_SIDE)/client.c
	$(CC) $(FLAGS) $(CLIENT_SIDE)/client.c -o $(OUT)/client.o
$(OUT)/client-ui.o : $(CLIENT_SIDE)/client-ui.c
	$(CC) $(FLAGS) $(CLIENT_SIDE)/client-ui.c -o $(OUT)/client-ui.o
$(OUT)/validate.o : $(CLIENT_SIDE)/validate.c
	$(CC) $(FLAGS) $(CLIENT_SIDE)/validate.c -o $(OUT)/validate.o
$(OUT)/postman.o : $(SERVER_SIDE)/postman.c
	$(CC) $(FLAGS) $(SERVER_SIDE)/postman.c -o $(OUT)/postman.o
$(OUT)/storage.o : $(SERVER_SIDE)/storage.c
	$(CC) $(FLAGS) $(SERVER_SIDE)/storage.c -o $(OUT)/storage.o
$(OUT)/utils.o: $(UTILS_DIR)/utils.c
	$(CC) $(FLAGS) $(UTILS_DIR)/utils.c -o $(OUT)/utils.o
$(OUT)/server.o : $(SERVER_SIDE)/server.c
	$(CC) $(FLAGS) $(SERVER_SIDE)/server.c -o $(OUT)/server.o

clean :
	rm $(OUT)/* $(EXEC)/*