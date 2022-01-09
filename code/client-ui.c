#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validate.h"
#include "postman.h"
#include "client-ui.h"

char username[100];
char password[100];

enum Phase currentPhase = NOT_LOGGED_IN;

void displayIntroScreen() {
	printf("Welcome to the Quizizz game monitor.\n");
	printf("Your UI version: CLI-1.1.0\n\n");

	printf("Copyright (c) 2021, 2022 - Network programming - final project - team 14.\n\n");

	printf("Type 'help' for help. Type 'quit' for quit.\n\n");
}

void runCLI(int sockfd) {
	displayIntroScreen();

	char command[100];
	do {
		printf("quizizz> "); scanf("%[^\n]s", command);
		while(getchar() != '\n');

		handleCommand(sockfd, command);
	} while(strcmp(command, "quit"));
}

int splitCommand(char* command, char* commandName, char** argv) {
	if (!command || !commandName || !argv) return -1;

	char* token = strtok(command, " ");
	if (token) strcpy(commandName, token);

	int argc = 0;
	token = strtok(NULL, " ");
	while (token) {
		argv[argc] = (char*) malloc(sizeof(char) * 100);
		strcpy(argv[argc], token);

		argc++;
		token = strtok(NULL, " ");	
	}

	return argc;
}

int commandIsValid(char* commandName) {
	// TODO - check command by current phase
	
	return 1;
}

void handleCommand(int sockfd, char* command) {
	char commandName[50];
	char* argv[10];
	int argc = splitCommand(command, commandName, argv);

	if (commandIsValid(commandName) < 0) return;

	switch(argc) {
		case 0:
			handleCommandNoArgument(sockfd, commandName);
			break;
		case 1:
			handleCommandOneArgument(sockfd, commandName, argv);
			break;
		case 2:
			handleCommandTwoArguments(sockfd, commandName, argv);
			break;
		default:
			printf("ERROR: Syntax error\n");
			printf("Please try to run 'help' for help.\n\n");
	}
}

void handleCommandNoArgument(int sockfd, char* commandName) {
	if (!strcmp(commandName, "help"))
		handleHelpCommand(NULL);

	else if (!strcmp(commandName, "quit"))
		handleQuitCommand();

	else if (!strcmp(commandName, "logout"))
		handleLogoutCommand(sockfd);

	else if (!strcmp(commandName, "lr"))
		handleLRCommand(sockfd);

	else if (!strcmp(commandName, "start"))
		handleStartCommand(sockfd);
	
	else printf("ERROR: Syntax error\n\n");
}

void handleLRCommand(int sockfd) {
	struct Request* req = createRequest(LR, NULL);
	sendRequest(sockfd, request);

	struct Response* res = NULL;
	receiveResponse(sockfd, res);

	if (!res) {
		printf("ERROR: can't receive response from server\n\n");
		return;
	}

	if (res->status == OK) {
		char* rooms[40];
		int n = split(res->data, "|", rooms);

		for (int i = 0; i < n; i++) {
			char* tokens[20];
			int m = split(rooms[i], " ", tokens);
			
			if (m != 3) {
				printf("Warning: have a problem in handleLRCommand() method\n\n");
				exit(1);
			}

			printf("------------- %s\n", tokens[0]); 	// room name
			printf("- host: %s\n", tokens[1]); 			// host name
			printf("- player: %s\n\n", tokens[2]); 		// number of player
		}
	}
	printf("%s\n\n", res->message);
}

void handleStartCommand(int sockfd) {
	currentPhase = PLAY_GAME;
	// TODO - send request to start game
}

void handleQuitCommand() {
	printf("\nBye\n");
}

void handleLogoutCommand(int sockfd) {
	struct Request* req = createRequest(LOGOUT, username);
	sendRequest(sockfd, req);

	struct Response* res = NULL;
	receiveResponse(sockfd, res);
	
	if (!res) {
		printf("ERROR: can't receive response from server\n\n");
		return;
	}
	if (res->status == OK) printf("Logout successful\n\n");

	strcpy(username, "");
	strcpy(password, "");
	currentPhase = NOT_LOGGED_IN;
}

void handleCommandOneArgument(int sockfd, char* commandName, char** argv) {
	if (!strcmp(commandName, "help"))
		handleHelpCommand(commandName);

	else if (!strcmp(commandName, "join"))
		handleJoinCommand(sockfd, argv);

	else if (!strcmp(commandName, "answer"))
		handleAnswerCommand(sockfd, argv);
	
	else printf("ERROR: Syntax error\n\n");
}

void handleHelpCommand(char* commandName) {
	if (!commandName) {
		// TODO - show all help
	} else {
		// TODO - show detail help for commandName
	}
}

void handleJoinCommand(int sockfd, char** argv) {
	// TODO - request to join into a room

	currentPhase = JOINED_ROOM;
}

void handleAnswerCommand(int sockfd, char** argv) {
	// TODO - request to submit answer
}

void handleCommandTwoArguments(int sockfd, char* commandName, char** argv) {

}

void handleLoginCommand(int sockfd, char** argv) {
	char message[100];
	strcpy(message, argv[0]);
	strcat(message, " ");
	strcat(message, argv[1]);

	struct Request* req = createRequest(LOGIN, message);
	sendRequest(sockfd, req);

	struct Response* res = NULL;
	receiveResponse(sockfd, res);
	
	if (!res) {
		printf("ERROR: can't receive response from server\n\n");
		return;
	}

	if (res->status == LOGIN_SUCCESS) {
		strcpy(username, argv[0]);
		strcpy(password, argv[1]);
		currentPhase = LOGGED_IN;
	}
	
	printf("%s\n\n", res->message);
}

void handleRegisterCommand(int sockfd, char** argv) {
	char message[100];
	strcpy(message, argv[0]);
	strcat(message, " ");
	strcat(message, argv[1]);

	struct Request* req = createRequest(REGISTER, message);
	sendRequest(sockfd, req);

	struct Response* res = NULL;
	receiveResponse(sockfd, res);
	
	if (!res) {
		printf("ERROR: can't receive response from server\n\n");
		return;
	}

	printf("%s\n\n", res->message);
}

void handleCRCommand(int sockfd, char** argv) {
	// TODO - request to create room
}














