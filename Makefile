all:
	gcc shelby.c client-ui.c validate.c postman.c -o client-ui

clean:
	rm client-ui
