CC = gcc
FLAGS = -Wall -g -c

OUT = out
EXEC = exec

SRC = src

all:  $(EXEC)/client $(EXEC)/server

$(EXEC)/client : $(OUT)/client.o $(OUT)/validate.o $(OUT)/client-ui.o $(OUT)/postman.o $(OUT)/utils.o
	$(CC) $(OUT)/client.o $(OUT)/validate.o $(OUT)/client-ui.o $(OUT)/postman.o $(OUT)/utils.o -o $(EXEC)/client
$(EXEC)/server : $(OUT)/server.o $(OUT)/storage.o $(OUT)/postman.o $(OUT)/utils.o
	$(CC) $(OUT)/server.o $(OUT)/storage.o $(OUT)/postman.o $(OUT)/utils.o -o $(EXEC)/server

$(OUT)/client.o : $(SRC)/client.c
	$(CC) $(FLAGS) $(SRC)/client.c -o $(OUT)/client.o
$(OUT)/client-ui.o : $(SRC)/client-ui.c
	$(CC) $(FLAGS) $(SRC)/client-ui.c -o $(OUT)/client-ui.o
$(OUT)/validate.o : $(SRC)/validate.c
	$(CC) $(FLAGS) $(SRC)/validate.c -o $(OUT)/validate.o
$(OUT)/postman.o : $(SRC)/postman.c
	$(CC) $(FLAGS) $(SRC)/postman.c -o $(OUT)/postman.o
$(OUT)/storage.o : $(SRC)/storage.c
	$(CC) $(FLAGS) $(SRC)/storage.c -o $(OUT)/storage.o
$(OUT)/utils.o: $(SRC)/utils.c
	$(CC) $(FLAGS) $(SRC)/utils.c -o $(OUT)/utils.o
$(OUT)/server.o : $(SRC)/server.c
	$(CC) $(FLAGS) $(SRC)/server.c -o $(OUT)/server.o

clean :
	rm $(OUT)/* $(EXEC)/*