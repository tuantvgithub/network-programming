#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	LOGIN,
	LOGOUT,
	REGISTER,

	JOIN,
	OUT,
	LR, SR, CR, DR,

	START,
	GET_EXAM,
	ANSWER
} Opcode;

struct Request {
	Opcode opcode;
	char message[100];
};

struct Request* createRequest(Opcode opcode, char* message);

void sendRequest(int sockfd, struct Request* request);
void receiveRequest(int sockfd, struct Request* request);


typedef enum {
	OK = 01,

	SYNTAX_ERROR = 11,
	SERVER_ERROR = 12,
	
	LOGIN_FAILED = 21,
	
	REGISTER_FAILED = 41,

	NO_CONTENT = 51
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
