#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validate.h"
#include "postman.h"
#include "client-ui.h"

char currentUsername[100] = "";
char currentPassword[100] = "";


void homePageScreen(int sockfd) {
	int choice;
	while(1) {
		printf("\n");
		printf(" __________ Home_page __________\n");
		printf("|                               |\n");
		printf("| 1. Login                      |\n");
		printf("| 2. Register                   |\n");
		printf("| 0. Exit                       |\n");
		printf("|_______________________________|\n");
		while(1) {
			printf("--\n");
			printf("---> Your choice: ");
			scanf("%d", &choice);
			while(getchar() != '\n');
			
			switch (choice) {
				case 1:
					loginScreen(sockfd);
					break;
				case 2:
					registerScreen(sockfd);
					break;
				case 0:
					exitGameScreen();
					break;
				default:
					printf("[Error! try again]");
			}
			if (choice > -1 && choice < 3) break;
		}
		if (choice == 0) break;
	}
}


void loginScreen(int sockfd) {
	char _username[100] = "";
	char _password[100] = "";
	
	printf("\n __________ Login __________\n");
	
	printf("--\n");
	printf("---> username: ");
	scanf("%[^\n]s", _username);
	while(getchar() != '\n');
	
	printf("---> password: ");
	scanf("%[^\n]s", _password);
	while(getchar() != '\n');

	printf("\n");

	char message[100]= "";
	strcpy(message, _username);
	strcat(message, " ");
	strcat(message, _password);
	struct Request* request = createRequest(LOGIN, message);

	sendRequest(sockfd, request, sizeof(request->message), 0);
	return;
	struct Response* response = (struct Response*) malloc(sizeof(struct Response));
	receiveResponse(sockfd, response, sizeof(response), 0);
	
	if (response->status == LOGIN_SUCCESS) {
		strcpy(currentUsername, _username);
		strcpy(currentPassword, _password);	

		printf("[Login successful]\n");
		greetingScreen();
		menuScreen();		 
	}
	else
		printf("[Login failed]\n");
}

void registerScreen(int sockfd) {
	char _username[100] = "";
	char _password[100] = "";
	
	printf("\n __________ Register _________\n");
	
	while(1) {
		printf("--\n");
		printf("---> username: ");
		scanf("%[^\n]s", _username);
		while(getchar() != '\n');

		if (validateUsername(_username) == 0)
			printf("[Try again]\n");
		else break;
	}
	
	while(1) {
		printf("---> password: ");
		scanf("%[^\n]s", _password);
		while(getchar() != '\n');
		
		if (validatePassword(_password) == 0)
			printf("[Try again]\n");
		else break;		
	}
	
	// int registerSuccess = registerNewAccount(_username, _password);
	// if (registerSuccess)
	// 	printf("[Registration successful]\n");
	// else
	// 	printf("[Sorry! registration failed]\n");
}

void exitGameScreen() {
	printf("\n[Bye!]\n\n");
}

void greetingScreen() {
	printf("\n[Hello %s]\n", currentUsername);
}

void menuScreen() {
	int choice = 0;	
	
	while(1) {
		printf("\n");
		printf(" ___________ Menu ______________\n");
		printf("|                               |\n");	
		printf("| 1. List room                  |\n");
		printf("| 2. Join room                  |\n");
		printf("| 3. Create room                |\n");
		printf("| 4. Logout                     |\n");
		printf("|_______________________________|\n");
		while (1) {
			printf("--\n");
			printf("---> Your choice: ");
			scanf("%d", &choice);
			while(getchar() != '\n');
			
			switch (choice) {
				case 1:
					listRoomScreen();
					break;
				case 2:
					joinRoomScreen();
					break;
				case 3:
					createRoomScreen();
					break;
				case 4:
					logoutScreen();
					break;
				default:
					printf("[Error! try again]\n");
			}
			if (choice > 0 && choice < 5) break;
		}
		if (choice == 4) break;
	}
}

void listRoomScreen() {
	printf("\n __________ List room __________\n");
}

void joinRoomScreen() {
	printf("\n __________ Join room __________\n");	
}

void createRoomScreen() {
	printf("\n __________ Create room __________\n");
}

void logoutScreen() {
	printf("\n[Logout successful]\n");
}
















