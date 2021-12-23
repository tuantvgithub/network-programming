#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "postman.h"

extern int sockfd;
Request createRequest(Opcode op, char* message) {
    Request req;
    req.opcode = op;
    if (strlen(message) > 0) {
        strcat(req.message, " ");
        strcat(req.message, message);
    }
    return req;
}

Response createResponse(MessageStatus status, char* message) {
    Response res;
    res.status = status;
    if (strlen(message) > 0) {
        strcat(res.message, " ");
        strcat(res.message, message);
    }
    return res;
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
    sendReq(sockfd, req);
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

char *opToString(Opcode op) {
    switch(op) {
        case USER:
            return "USER";
            break;
        case LOGIN:
            return "LOGIN";
            break;
        case REGISTER:
            return "REGISTER";
            break;
        case LOGOUT:
            return "LOGOUT";
            break;
    }
    return NULL;
}

char *statusToString(MessageStatus status) {
    switch(status) {
    case SYNTAX_ERROR:
        return "01";
        break;
    case USERNAME_FOUND:
        return "11";
        break;
    case USERNAME_NOT_FOUND:
        return "12";
        break;
    case LOGIN_FAILED:
        return "21";
        break;
    case LOGIN_SUCCESS:
        return "22";
        break;
    case REGISTER_FAILED:
        return "31";
        break;
    case REGISTER_SUCCESS:
        return "32";
        break;
    case LOGOUT_SUCCESS:
        return "41";
        break;
    }
    return NULL;
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

void sendReq(int sockfd, Request req) {
    char s[200];
    strcpy(s, opToString(req.opcode));
    if (strlen(req.message) > 0) {
        strcat(s, " ");
        strcat(s, req.message);
    }
    send(sockfd, s, strlen(s), 0);
}

// Not contain res.data
void sendRes(int sockfd, Response res) {
    char s[200];
    strcpy(s, statusToString(res.status));
    if (strlen(res.message) > 0) {
        strcat(s, " ");
        strcat(s, res.message);
    }
    // if (res.data != NULL) {
    // }
    send(sockfd, s, strlen(s), 0);
}
