#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/validate.h"

int validateUsername(char* username) {
	int len = strlen(username);
	if (len == 0) return -1;
	
	for (int i = 0; i < len; i++)
		if (username[i] == ' ' || username[i] == '\n') return -1;
	
	return 1;
}

int validatePassword(char* password) {
	int len = strlen(password);
	if (len == 0) return -1;
	if (len < 6) return -1;
	
	for (int i = 0; i < len; i++)
		if (password[i] == ' ' || password[i] == '\n')
			return -1;
	
	return 1;
}

int validateRoomName(char* roomName) {
	if (!roomName) return -1;
	int len = strlen(roomName);
	if (len == 0) return -1;

	for (int i = 0; i < len; i++) 
		if (roomName[i] == ' ')
			return -1;

	return 1;
}

int validateQuestionFileName(char* questionFileName) {
	if (!questionFileName) return -1;
	int len = strlen(questionFileName);
	if (len == 0) return -1;

	for (int i = 0; i < len; i++) 
		if (questionFileName[i] == ' ')
			return -1;

	return 1;
}

int validateAnswerFileName(char* answerFileName) {
	if (!answerFileName) return -1;
	int len = strlen(answerFileName);
	if (len == 0) return -1;

	for (int i = 0; i < len; i++) 
		if (answerFileName[i] == ' ')
			return -1;

	return 1;
}







