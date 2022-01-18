#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	LOGIN,
	LOGOUT,
	REGISTER,

	JOIN,
	OUT,
	LR, SR, CR, DR, JR,

	START,
	STOP,
	GET_EXAM,
	GET_RESULT,
	ANSWER
} Opcode;

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
	
	LOGIN_FAILED = 21,
	
	REGISTER_FAILED = 41,

	NO_CONTENT = 51, 

	CREATE_ROOM_FAILED = 71,
	DROP_ROOM_FAILED = 72,
	ROOM_NOT_FOUND = 73,

	START_FAILED = 81,
	STOP_EXAM_FAILED = 82,
	GET_EXAM_FAILED = 83,

	ANSWER_FAILED = 91
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
