#ifndef __POSTMAN_H__
#define __POSTMAN_H__

typedef enum {
	LOGIN,
	REGISTER,
	LOGOUT
} Opcode;

typedef enum {
	OK = 01,

	SYNTAX_ERROR = 11,
	
	LOGIN_FAILED = 21,
	LOGIN_SUCCESS = 22
} ResponseStatus;

struct Request {
	Opcode opcode;
	char message[100];
};

struct Response {
	ResponseStatus status;
	char message[100];
	char data[1000];
};

/* Functions to handle with Request */
struct Request* createRequest(Opcode opcode, char* message);

void sendRequest(int sockfd, struct Request* request);

char* makeBufferFromRequest(struct Request* request);
struct Request* makeRequestFromBuffer(char* buff);
/* End */

/* Functions to handle with Response */
struct Response* createResponse(ResponseStatus status, char* message);
void setResponseMessage(struct Response* response);

void sendResponse(int sockfd, struct Response* response);

char* makeBufferFromResponse(struct Response* response);
struct Response* makeResponseFromBuffer(char* buff);
/* End */

#endif // __VALIDATE_H__
