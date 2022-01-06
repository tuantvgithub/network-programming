#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"
#define MAXLEN 10000

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
    char *buf = (char*) malloc(sizeof(char)* MAXLEN );
    sprintf(buf, "%d %s", request->opcode, request->message);
    printf("mess: %s\n", buf);
    if (send(sockfd, buf, strlen(buf), flags) < 0)
        perror("Error");
    free(buf);
}

void receiveRequest(int sockfd, struct Request* request, int size, int flags) {
    char *buf = (char*) malloc(sizeof(char)*MAXLEN);
    if (recv(sockfd, buf, MAXLEN, 0) < 0) {
        free(buf);
        return;
    }
    printf("%s.\n", buf);

    request->opcode = atoi(strtok(buf, " "));
    char *a = strtok(NULL, "");
    if (a)
        strcpy(request->message, a);
    free(buf);
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

void sendResponse(int sockfd, struct Response* response) {
    char *buf = (char*) malloc(sizeof(char)* MAXLEN );
    sprintf(buf, "%d %s", response->status, response->data);
    printf("send res: %s\n", buf);
    if (send(sockfd, buf, strlen(buf), 0) < 0)
        perror("Error");
    free(buf);
}

void receiveResponse(int sockfd, struct Response* response, int size, int flags) {
    char *buf = (char*) malloc(sizeof(char) * MAXLEN);
    if ( recv(sockfd, buf, MAXLEN, 0) < 0) {
        return;
    }
    printf("%s.\n", buf);

    response->status = atoi(strtok(buf, " "));
    strcpy(response->message, "");
    
    char *a = strtok(NULL, "");
    if (a)
        strcpy(response->data, a);
}