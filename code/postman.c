#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"
#define MAXLEN 10000

struct Request* createRequest(Opcode op, char* message) {
    struct Request* request = (struct Request*) malloc(sizeof(struct Request));
    if (!request) printf("Error: Failed to allocate memory when creating Request\n\n");

    request->opcode = op;
    if (message)
        strcpy(request->message, message);

    return request;
}

void sendRequest(int sockfd, struct Request* request) {
    char *bufff = (char*) malloc(sizeof(char)* MAXLEN );
    sprintf(bufff, "%d %s", request->opcode, request->message);
    
    // printf("send mess: %s\n", bufff);

    if (send(sockfd, bufff, strlen(bufff), 0) < 0) perror("Error: ");
    free(bufff);
}

void receiveRequest(int sockfd, struct Request* request) {
    if (!request) request = (struct Request*) malloc(sizeof(struct Request));

    char *bufff = (char*) malloc(sizeof(char)*MAXLEN);
    if (recv(sockfd, bufff, MAXLEN, 0) < 0) {
        free(bufff);
        return;
    }

    request->opcode = atoi(strtok(bufff, " "));
    char *a = strtok(NULL, "");
    if (a)
        strcpy(request->message, a);
    free(bufff);
}

void setResponseMessage(struct Response* response) {
    if (!response) return;
    
    int messStatus = response->status;

    switch(messStatus) {
        case OK:
            strcpy(response->message, "OK");
            break;
        case SYNTAX_ERROR:
            strcpy(response->message, "Syntax error");
            break;
        case SERVER_ERROR:
            strcpy(response->message, "Server error");
            break;  
        case LOGIN_SUCCESS:
            strcpy(response->message, "Login successful");
            break;
        case LOGIN_FAILED:
            strcpy(response->message, "Login failed");
            break;
        case REGISTER_FAILED:
            strcpy(response->message, "Register failed");
            break;
        case REGISTER_SUCCESS:
            strcpy(response->message, "Register successful");
            break;
        case NO_CONTENT:
            strcpy(response->message, "No content");
            break;
        default:
            strcpy(response->message, "Exception");
    }
}

struct Response* createResponse(MessageStatus status, char* data) {
    struct Response* response = (struct Response*) malloc(sizeof(struct Response));
    if (!response) printf("Error: Failed to allocate memory when creating Response\n\n");

    response->status = status;
    setResponseMessage(response);
    if (data) strcpy(response->data, data);

    return response;
}

void sendResponse(int sockfd, struct Response* response) {
    char *buff = (char*) malloc(sizeof(char)* MAXLEN );
    sprintf(buff, "%d %s", response->status, response->data);

    // printf("send res: %s\n", buff);
    
    if (send(sockfd, buff, strlen(buff), 0) < 0)
        perror("Error");
    
    free(buff);
}

void receiveResponse(int sockfd, struct Response* response) {
	if (!response) response = (struct Response*) malloc(sizeof(struct Response));

    char *buff = (char*) malloc(sizeof(char) * MAXLEN);
    if ( recv(sockfd, buff, MAXLEN, 0) < 0) {
        return;
    }
    printf("%s.\n", buff);

    response->status = atoi(strtok(buff, " "));
    setResponseMessage(response);
    
    char *a = strtok(NULL, "");
    if (a)
        strcpy(response->data, a);
    free(buff);
}