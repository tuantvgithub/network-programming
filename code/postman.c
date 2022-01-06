#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"

extern int sockfd;
struct Request* createRequest(Opcode op, char* message) {
    struct Request* request = (struct Request*) malloc(sizeof(struct Request));
    if (!request) printf("Failed to allocate memory when creating Request");

    request->opcode = op;
    if (message)
        strcpy(request->message, message);

    return request;
}

void sendRequest(int sockfd, struct Request* request, int size, int flags) {
    printf("mess: %s\n", request->message);
    if (send(sockfd, request->message, size, flags) < 0)
        perror("Error");
}

void setResponseMessage(struct Response* response) {
    if (!response) return;
    
    int messStatus = response->status;

    switch(messStatus) {
        case LOGIN_FAILED:
            strcpy(response->message, "Login failed");
            break;
        case LOGIN_SUCCESS:
            strcpy(response->message, "Login successful");
            break;
        default:
            strcpy(response->message, "Exception");
    }
}

struct Response* createResponse(MessageStatus status, char* data) {
    struct Response* response = (struct Response*) malloc(sizeof(struct Response));
    if (!response) printf("Failed to allocate memory when creating Response");

    response->status = status;
    setResponseMessage(response);
    if (data) strcpy(response->data, data);

    return response;
}

void receiveResponse(int sockfd, struct Response* response, int size, int flags) {

}