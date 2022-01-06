#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "postman.h"
#include "utils.h"


struct Request* createRequest(Opcode op, char* message) {
    struct Request* request = (struct Request*) malloc(sizeof(struct Request));
    if (!request) printError("Failed to allocate memory when creating Request");

    request->opcode = op;
    if (message)
        strcpy(request->message, message);

    return request;
}

void sendRequest(int sockfd, struct Request* request) {
    if (sockfd < 0 || !request)
        printError("Can't send request");
    
    char* buff = makeBufferFromRequest(request);
    if (!buff)
        printError("Failed to make buffer");


    if (send(sockfd, buff, sizeof(buff), 0) < 0)
        perror("Error: ");
}

char* makeBufferFromRequest(struct Request* request) {
    if (!request) return NULL;
    char* buff = (char*) malloc(sizeof(char) * 100);
    Opcode opcode = request->opcode;
    
    switch (opcode) {
        case LOGIN:
            strcpy(buff, "LOGIN");
            break;
        case REGISTER:
            strcpy(buff, "REGISTER");
            break;
        default:
            return NULL;
    }
    if (strlen(request->message) > 0)
        strcat(buff, " ");
    strcat(buff, request->message);

    return buff;
}

struct Request* makeRequestFromBuffer(char* buff) {
    if (!buff) return NULL;
    struct Request* request = (struct Request*) malloc(sizeof(struct Request));
    if (!request) {
        printError("Failed to allocate memory");
        return NULL;
    }

    char _buff[100]; strcpy(_buff, buff);

    char* token = strtok(_buff, " ");
    char* message = subString(buff, strlen(token) + 1, 100);

    strcpy(request->message, message);
    if (!strcmp(token, "LOGIN"))
        request->opcode = LOGIN;
    else if (!strcmp(token, "REGISTER"))
        request->opcode = REGISTER;
    else {
        free(request);
        return NULL;
    }

    return request;
}

struct Response* createResponse(ResponseStatus status, char* data) {
    struct Response* response = (struct Response*) malloc(sizeof(struct Response));
    if (!response) printError("Failed to allocate memory when creating Response");

    response->status = status;
    setResponseMessage(response);
    if (data) strcpy(response->data, data);

    return response;
}

void setResponseMessage(struct Response* response) {
    if (!response) return;
    ResponseStatus status = response->status;

    switch (status) {
        case OK:
            strcpy(response->message, "[OK]");
            break;
        case LOGIN_SUCCESS:
            strcpy(response->message, "[LOGIN_SUCCESS]");
            break;        
        case LOGIN_FAILED:
            strcpy(response->message, "[LOGIN_FAILED]");
            break;
        default:
            return;
    }
}

void sendResponse(int sockfd, struct Response* response) {
    if (sockfd < 0 || !response)
        printError("Can't send response");
    
    char* buff = makeBufferFromResponse(response);
    if (!buff)
        printError("Failed to make buffer");

    if (send(sockfd, buff, sizeof(buff), 0) < 0)
        perror("Error: ");
}

char* makeBufferFromResponse(struct Response* response) {
    if (!response) return NULL;
    char* buff = (char*) malloc(sizeof(char) * 1000);
    ResponseStatus status = response->status;

    if (strlen(response->message) > 0)
        strcpy(buff, response->message);

    if (strlen(response->data) > 0)
        strcat(buff, "|");
    strcat(buff, response->data);

    return buff;
}

struct Response* makeResponseFromBuffer(char* buff) {
    if (!buff) return NULL;
    struct Response* response = (struct Response*) malloc(sizeof(struct Response));
    if (!response) {
        printError("Failed to allocate memory");
        return NULL;
    }

    char _buff[1000]; strcpy(_buff, buff);
    char* message = strtok(_buff, "|");
    strcpy(response->message, message);

    if (!strcmp(message, "[OK]"))
        response->status = OK;
    else if (!strcmp(message, "[LOGIN_SUCCESS]"))
        response->status = OK;
    else if (!strcmp(message, "[LOGIN_FAILED]"))
        response->status = OK;
    else {
        free(response);
        return NULL;
    }

    char* data = subString(buff, strlen(message) + 1, 100);
    strcpy(response->data, data);

    return response;
}
