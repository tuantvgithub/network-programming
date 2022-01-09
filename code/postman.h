#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	LOGIN,
	LOGOUT,
	REGISTER,

	LR
} Opcode;

typedef enum {
	OK = 01,

	SYNTAX_ERROR = 11,
	SERVER_ERROR = 12,
	
	LOGIN_FAILED = 21,
	LOGIN_SUCCESS = 22,
	
	REGISTER_FAILED = 41,
	REGISTER_SUCCESS = 42,

	NO_CONTENT = 51
} MessageStatus;

struct Request {
	Opcode opcode;
	char message[100];
};

struct Response {
	MessageStatus status;
	char message[100];
	char data[1000];
};

struct Request* createRequest(Opcode opcode, char* message);

void sendRequest(int sockfd, struct Request* request);
void receiveRequest(int sockfd, struct Request* request);


struct Response* createResponse(MessageStatus messStatus, char* data);
void setResponseMessage(struct Response* response);

void sendResponse(int sockfd, struct Response* response);
void receiveResponse(int sockfd, struct Response* response);

#endif // __POSTMAN_H__
