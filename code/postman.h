#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
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

struct Request {
	Opcode opcode;
	char message[100];
};

struct Response {
	MessageStatus status;
	char message[100];
	char data[100];
};

struct Request* createRequest(Opcode opcode, char* message);
void sendRequest(int sockfd, struct Request* request, int size, int flags);

struct Response* createResponse(MessageStatus messStatus, char* data);
void setResponseMessage(struct Response* response);
void receiveResponse(int sockfd, struct Response* response, int size, int flags);

#endif // __VALIDATE_H__
