#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"
#include "../util/utils.h"


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
    char *buff = (char*) malloc(sizeof(char)* MAXLEN );
    strcpy(buff, "");

    sprintf(buff, "%d %s", request->opcode, request->message);
    
    // printf("send mess: %s\n", buff);

    if (send(sockfd, buff, strlen(buff), 0) < 0)
        printf("\n-> Error: can't send request.\n\n");

    free(buff);
}

int receiveRequest(int sockfd, struct Request* request) {
    char *buff = (char*) malloc(sizeof(char)*MAXLEN);
    strcpy(buff, "");

    if (recv(sockfd, buff, MAXLEN, 0) < 0) {
        printf("\n-> Error: can't receive request.\n\n");
        free(buff);
        return -1;
    }

    // printf("%s....\n", buff);

    request->opcode = atoi(strtok(buff, " "));
    char *a = strtok(NULL, "");
    if (a) strcpy(request->message, a);
    int size = strlen(buff);

    free(buff);
    return size;
}

void setResponseMessage(struct Response* response) {
    if (!response) return;
    
    int status = response->status;

    switch(status) {
        case OK:
            strcpy(response->message, "OK");
            break;
        case SYNTAX_ERROR:
            strcpy(response->message, "Syntax error");
            break;
        case SERVER_ERROR:
            strcpy(response->message, "Server error");
            break;  
        case LOGIN_FAILED:
            strcpy(response->message, "Login failed");
            break;
        case REGISTER_FAILED:
            strcpy(response->message, "Register failed");
            break;
        case NO_CONTENT:
            strcpy(response->message, "No content");
            break;
        case CREATE_ROOM_FAILED:
            strcpy(response->message, "Create room failed");
            break;
        case DROP_ROOM_FAILED:
            strcpy(response->message, "Drop room failed");
            break;
        default:
            strcpy(response->message, "Exception");
    }
}

struct Response* createResponse(ResponseStatus status, char* data) {
    struct Response* response = (struct Response*) malloc(sizeof(struct Response));
    if (!response) printf("Error: Failed to allocate memory when creating Response\n\n");
    
    response->status = status;
    setResponseMessage(response);
    if (data) strcpy(response->data, data);
    else strcpy(response->data, "");
    
    return response;
}

void sendResponse(int sockfd, struct Response* response) {
    char *buff = (char*) malloc(sizeof(char)* MAXLEN );
    strcpy(buff, "");
    
    if (!response->data || strlen(response->data) == 0)
        sprintf(buff, "%d", response->status);
    else
        sprintf(buff, "%d|%s", response->status, response->data);

    // printf("send res: %s\n", buff);
    
    if (send(sockfd, buff, strlen(buff), 0) < 0)
        printf("\n-> Error: can't send response.\n\n");
    
    free(buff);
}

void receiveResponse(int sockfd, struct Response* response) {
    char *buff = (char*) malloc(sizeof(char) * MAXLEN);
    strcpy(buff, "");

    if (recv(sockfd, buff, MAXLEN, 0) < 0) {
        printf("\n-> Error: can't receive response.\n\n");
        free(response);
        return;
    }

    char* tokens[5];
    int n = split(buff, "|", tokens);
    
    response->status = atoi(tokens[0]);
    setResponseMessage(response);
    if (n > 1) strcpy(response->data, tokens[1]);
}