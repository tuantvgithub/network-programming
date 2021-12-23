#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validate.h"
#include "postman.h"
#include "client-ui.h"

char currentUsername[100] = "";
char currentPassword[100] = "";

void homePageScreen() {
	int choice;
	
	while(1) {
		printf("\n");
		printf(" __________ Home_page __________\n");
		printf("|                               |\n");
		printf("| 1. Login                      |\n");
		printf("| 2. Register                   |\n");
		printf("| 0. Exit                       |\n");
		printf("|_______________________________|");
		while(1) {
			printf("\n---> your choice: ");
			scanf("%d", &choice);
			while(getchar() != '\n');
			
			switch (choice) {
				case 1:
					loginScreen();
					break;
				case 2:
					registerScreen();
					break;
				case 0:
					exitGameScreen();
					break;
				default:
					printf("[error! try again.]");
			}
			if (choice > -1 && choice < 3) break;
		}
		if (choice == 0) break;
	}
}

void loginScreen() {
	char _username[100] = "";
	char _password[100] = "";
	
	printf("\n __________ Login __________\n");
	
	printf("---> username: ");
	scanf("%[^\n]s", _username);
	while(getchar() != '\n');
	
	printf("---> password: ");
	scanf("%[^\n]s", _password);
	
	int loginSuccess = login(_username, _password);
	if (loginSuccess) {
		strcpy(currentUsername, _username);
		strcpy(currentPassword, _password);	

		printf("[login successful.]\n");
		greetingScreen();

		menuScreen();		 
	}
	else
		printf("[login failed.]\n");
}

void registerScreen() {
	char _username[100] = "";
	char _password[100] = "";
	
	printf("\n __________ Register _________\n");
	
	while(1) {
		printf("--\n");
		printf("---> username: ");
		scanf("%[^\n]s", _username);
		while(getchar() != '\n');

		if (validateUsername(_username) == 0)
			printf("[try again.]\n");
		else break;
	}
	
	while(1) {
		printf("---> password: ");
		scanf("%[^\n]s", _password);
		while(getchar() != '\n');
		
		if (validatePassword(_password) == 0)
			printf("[try again.]\n");
		else break;		
	}
	
	int registerSuccess = registerNewAccount(_username, _password);
	if (registerSuccess)
		printf("[registration successful.]\n");
	else
		printf("[sorry! registration failed.]\n");
}

void exitGameScreen() {
	printf("[Bye!]\n");
}

void greetingScreen() {
	printf("[Hello %s.]\n", currentUsername);
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
		printf("|_______________________________|");
		while (1) {
			printf("\n---> your choice: ");
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
					printf("[error! try again.]\n");
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
	printf("\n __________ Logout __________\n");
}
















