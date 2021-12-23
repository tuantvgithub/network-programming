#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "postman.h"
#include "client.h"

extern int sockfd;
Request createRequest(Opcode op, char* message) {
    Request req;
    req.opcode = op;
    switch(op) {
        case USER:
            strcat(req.message, "USER");
            break;
        case LOGIN:
            strcat(req.message, "LOGIN");
            break;
        case REGISTER:
            strcat(req.message, "REGISTER");
            break;
        case LOGOUT:
            strcat(req.message, "LOGOUT");
            break;
    }
    if (strlen(message) > 0) {
        strcat(req.message, " ");
        strcat(req.message, message);
    }
    return req;
}

Response createResponse(MessageStatus status, char* message) {
    Response res;
    res.status = status;
    // switch(status) {
    //     case USER:
    //         strcat(res.message, "USER");
    //         break;
    //     case LOGIN:
    //         strcat(res.message, "LOGIN");
    //         break;
    //     case REGISTER:
    //         strcat(res.message, "REGISTER");
    //         break;
    //     case LOGOUT:
    //         strcat(res.message, "LOGOUT");
    //         break;
    // }
    if (strlen(message) > 0) {
        strcat(res.message, " ");
    }
    strcat(res.message, message);
}

int login(char* username, char* password) {
    char message[100] = "";
    strcat(message, username);
    strcat(message, " ");
    strcat(message, password);

    Request req = createRequest(LOGIN, message);
    send(sockfd, req.message, strlen(req.message), 0);

	return 1;
}

int registerNewAccount(char* username, char* password) {
    char message[100] = "";
    strcat(message, username);
    strcat(message, " ");
    strcat(message, password);

    Request req = createRequest(REGISTER, message);
    send(sockfd, req.message, strlen(req.message), 0);
	return 1;
}


Opcode getOp(char* op) {
    if (!strcmp(op, "USER")) {
        return USER;
    } else if (!strcmp(op, "LOGIN")) {
        return LOGIN;
    } else if (!strcmp(op, "REGISTER")) {
        return REGISTER;
    } else if (!strcmp(op, "LOGOUT")) {
        return LOGOUT;
    }
    return -1;
}

MessageStatus getMessStatus(char* status) {
    // if (!strcmp(status, "11")) {
    //     return USERNAME_FOUND;
    // } else if (!strcmp(status, "12")) {
    //     return USERNAME_NOT_FOUND;
    // } else if (!strcmp(status, "21")) {
    //     return LOGIN_FAILED;
    // } else if (!strcmp(status, "22")) {
    //     return LOGIN_SUCCESS;
    // } else if (!strcmp(status, "31")) {
    //     return REGISTER_FAILED;
    // } else if (!strcmp(status, "32")) {
    //     return REGISTER_SUCCESS;
    // } else if (!strcmp(status, "41")) {
    //     return LOGOUT_SUCCESS;
    // } else if (!strcmp(status, "01")) {
    //     return SYNTAX_ERROR;
    // }
    // return -1;
    return atoi(status);
}

// void sendReq(int sockfd, Request req) {
//     char 
//     send(sockfd, req.message, strlen(req.message), 0);
// }

// void sendRes(int sockfd, Response res) {

// }