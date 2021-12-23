#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validate.h"

int validateUsername(char* username) {
	int len = strlen(username);
	if (len == 0) {
		printf("[username is required.]\n"); 
		return 0;
	}
	
	for (int i = 0; i < len; i++) {
		if (username[i] == ' ' || username[i] == '\n') {
			printf("[username must not contain spaces.]\n");
			return 0;
		}		
	}
	
	return 1;
}

int validatePassword(char* password) {
	int len = strlen(password);
	if (len == 0) {
		printf("[password is required.]\n");
		return 0;
	}
	if (len < 6) {
		printf("[password must contain at least 6 characters.]\n");
		return 0;
	}
	
	for (int i = 0; i < len; i++) {
		if (password[i] == ' ' || password[i] == '\n') {
			printf("[password must not contain spaces.]\n");
			return 0;
		}
	}
	
	return 1;
}







