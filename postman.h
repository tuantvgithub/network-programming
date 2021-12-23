#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	USER,
	LOGIN,
	REGISTER,
	LOGOUT
} Opcode;

typedef enum {
	SYNTAX_ERROR = 01,
	
	USERNAME_FOUND = 11,
	USERNAME_NOT_FOUND = 12,
	
	LOGIN_FAILED = 21,
	LOGIN_SUCCESS = 22,
	
	REGISTER_FAILED = 31,
	REGISTER_SUCCESS = 32,
	
	LOGOUT_SUCCESS = 41
} MessageStatus;

typedef struct {
	Opcode opcode;
	char message[100];
} Request;

typedef struct {
	MessageStatus status;
	char message[100];
	char data[100];
} Response;

int login(char* username, char* password);
int registerNewAccount(char* username, char* password);

Opcode getOp(char* op);
MessageStatus getMessStatus(char* messStatus);

#endif // __VALIDATE_H__
