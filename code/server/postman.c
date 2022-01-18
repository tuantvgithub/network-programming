#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"
#include "../util/utils.h"


#define MAXLEN 10000


char* readOpcode(Opcode op) {
    char* str = (char*) malloc(sizeof(char) * 100);

    switch(op) {
        case LOGIN:
            strcpy(str, "Login");
            break;
        case LOGOUT:
            strcpy(str, "Logout");
            break;
        case REGISTER:
            strcpy(str, "Register");
            break;
        case JOIN:
            strcpy(str, "Join room");
            break;
        case OUT:
            strcpy(str, "Out room");
            break;
        case LR:
            strcpy(str, "List room");
            break;
        case SR:
            strcpy(str, "Show room");
            break;
        case CR:
            strcpy(str, "Create room");
            break;
        case DR:
            strcpy(str, "Drop room");
            break;
        case START:
            strcpy(str, "Start exam");
            break;
        case STOP:
            strcpy(str, "Stop exam");
            break;
        case GET_EXAM:
            strcpy(str, "Get exam");
            break;
        case GET_RESULT:
            strcpy(str, "Get result");
            break;
        case ANSWER:
            strcpy(str, "Answer");
            break;
        default:
            strcpy(str, "Unknow");
    }

    return str;
}

struct Request* createRequest(Opcode op, char* message) {
    struct Request* request = (struct Request*) malloc(sizeof(struct Request));
    if (!request) printf("Error: Failed to allocate memory when creating Request\n\n");

    request->opcode = op;
    if (message)
        strcpy(request->message, message);

    return request;
}

void sendRequest(int sockfd, struct Request* request) {
    char buff[MAXLEN] = "";
    sprintf(buff, "%d %s", request->opcode, request->message);    
    // printf("send req: %s\n", buff);

    if (send(sockfd, buff, MAXLEN, 0) < 0)
        printf("\n-> Error: can't send request.\n\n");
}

int receiveRequest(int sockfd, struct Request* request) {
    char buff[MAXLEN] = "";
    if (recv(sockfd, buff, MAXLEN, 0) < 0) {
        printf("\n-> Error: can't receive request.\n\n");
        return -1;
    }
    // printf("recev req: %s\n", buff);

    request->opcode = atoi(strtok(buff, " "));
    char *a = strtok(NULL, "");
    if (a) strcpy(request->message, a);
    int size = strlen(buff);

    return size;
}

void setResponseMessage(struct Response* response) {
    if (!response) return;
    ResponseStatus status = response->status;

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
        case INCORRECT_PASSWORD:
            strcpy(response->message, "Incorrect password");
            break;
        case USERNAME_NOT_EXISTS:
            strcpy(response->message, "Username does not exists");
            break;
        case USERNAME_ALREADY_EXISTS:
            strcpy(response->message, "Username already exists");
            break;
        case ACCOUNT_ALREADY_LOGGED_IN:
            strcpy(response->message, "Account is logged in");
            break;
        case ROOM_NOT_EXISTS:
            strcpy(response->message, "Room not exists");
            break;
        case ROOM_ALREADY_EXISTS:
            strcpy(response->message, "Room already exists");
            break;
        case NOT_ALLOWED:
            strcpy(response->message, "You are not allowed");
            break;
        case EXAM_NOT_STARTED:
            strcpy(response->message, "Exam not started");
            break;
        case EXAM_ALREADY_FINISHED:
            strcpy(response->message, "Exam already finished");
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
    char buff[MAXLEN] = "";
    if (!response->data || strlen(response->data) == 0)
        sprintf(buff, "%d", response->status);
    else
        sprintf(buff, "%d&%s", response->status, response->data);

    // printf("send res: %s\n", buff);
    
    if (send(sockfd, buff, MAXLEN, 0) < 0)
        printf("\n-> Error: can't send response.\n\n");
}

void receiveResponse(int sockfd, struct Response* response) {
    char buff[MAXLEN] = "";
    if (recv(sockfd, buff, MAXLEN, 0) < 0) {
        printf("\n-> Error: can't receive response.\n\n");
        return;
    }
    // printf("buff: %s\n", buff);

    char* tokens[5];
    int n = split(buff, "&", tokens);

    response->status = atoi(tokens[0]);
    setResponseMessage(response);
    if (n > 1) strcpy(response->data, tokens[1]);
    else strcpy(response->data, "");

    freeArr(tokens, n);
}