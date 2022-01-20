#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	LOGIN, LOGOUT, REGISTER,

	JOIN, OUT, LR, SR, CR, DR,

	START, STOP, GET_EXAM, 
	GET_RESULT, ANSWER
} Opcode;

char* readOpcode(Opcode op);

struct Request {
	Opcode opcode;
	char message[100];
};

struct Request* createRequest(Opcode opcode, char* message);
void sendRequest(int sockfd, struct Request* request);
int receiveRequest(int sockfd, struct Request* request);


typedef enum {
	OK = 01,

	SYNTAX_ERROR = 11,
	SERVER_ERROR = 12,
	
	INCORRECT_PASSWORD = 21,
	USERNAME_NOT_EXISTS = 22,
	USERNAME_ALREADY_EXISTS = 23,
	ACCOUNT_ALREADY_LOGGED_IN = 24,

	ROOM_NOT_EXISTS = 31,
	ROOM_ALREADY_EXISTS = 32,

	NOT_ALLOWED = 41,

	EXAM_NOT_STARTED = 51,
	EXAM_ALREADY_FINISHED = 52,
} ResponseStatus;

struct Response {
	ResponseStatus status;
	char message[100];
	char data[1000];
};

struct Response* createResponse(ResponseStatus messStatus, char* data);
void setResponseMessage(struct Response* response);
void sendResponse(int sockfd, struct Response* response);
void receiveResponse(int sockfd, struct Response* response);

#endif // __POSTMAN_H__
