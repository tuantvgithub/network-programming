#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "postman.h"
#include "storage.h"
#include "../util/utils.h"
#include "server.h"

#define MAXLINE 4096 /*max text line length*/
#define LISTENQ 8 /*maximum number of client connections*/


int main (int argc, char **argv) {
    if (argc != 2) {
        perror("Usage: ./server portNumber"); 
        exit(1);
    }

    int listenfd;

    //Create a socket for the soclet
    //If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(1);
    }

    struct sockaddr_in servaddr;

    //preparation of the socket address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    //bind the socket
    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen (listenfd, LISTENQ);

    printf("Server running... in port %s. Waiting for connections.\n", argv[1]);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int connfd, rcvBytes = sizeof(clientAddr);
    pid_t childpid;

    while (1) {
        // accept a connection
        connfd = accept(listenfd, (struct sockaddr *) &clientAddr, &clientAddrLen);

        if ((childpid = fork()) == 0) {
            close(listenfd);

            while (1) {
                struct Request* req = (struct Request*) malloc(sizeof(struct Request));
                if ((rcvBytes = receiveRequest(connfd, req)) <= 0)  break;
                
                printf("Client[%s:%d]:\n",inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                printf("Request:\n  -opcode: %d\n  -message: %s\n", req->opcode, req->message);

                struct Response* res = handleRequest(connfd, req);
                printf("Response:\n  -message: %s\n  -data: %s\n", res->message, 
                            (strlen(res->data) == 0 ? "NULL" : res->data));
                printf("--------------------------------------\n");

                sendResponse(connfd, res);
                free(req);
                free(res);
            }
            if (rcvBytes < 0)
                printf("Read error\n\n");
            exit(1);
        }
    }
}

struct Response* handleRequest(int connfd, struct Request *request) {
    if (!request || !request->message) 
        return createResponse(SYNTAX_ERROR, NULL);

    switch (request->opcode) {
        case LOGIN:
            return login(request);
        case LOGOUT:
            return logout(request);
        case REGISTER:
            return doRegister(request);
        case CR:
            return createRoom(request);
        case DR:
            return dropRoom(request);
        case LR:
            return listRoom(request);
        case SR:
            return showRoom(request);
        case JOIN:
            return joinRoom(request);
        case OUT:
            return outRoom(request);
        case START:
            return startExam(request);
        case GET_EXAM:
            return getExam(request);
        case ANSWER:
            return answer(request);
        default:
            return createResponse(SYNTAX_ERROR, NULL);
    }
}

struct Response* login(struct Request* req) {    
    char* tokens[5];
    int n = split(req->message, " ", tokens);
    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Account* account = getAccountByUsername(tokens[0]);
    if (!account || strcmp(account->password, tokens[1]))
        return createResponse(LOGIN_FAILED, NULL);

    if (accountIsActive(tokens[0]) > 0)
        return createResponse(LOGIN_FAILED, NULL);

    saveActiveAccount(tokens[0]);

    char data[1000] = "";
    strcpy(data, account->role);

    free(account);
    freeArr(tokens, n);
    return createResponse(OK, data);
}

struct Response* logout(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);
    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);

    deleteActiveAccount(tokens[0]);

    freeArr(tokens, n);
    return createResponse(OK, NULL);
}

struct Response* doRegister(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);
    if (n != 3)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Account* account = getAccountByUsername(tokens[0]);
    if (account)
        return createResponse(REGISTER_FAILED, NULL);
    
    if (saveAccount(tokens[0], tokens[1], tokens[2]) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    free(account);
    return createResponse(OK, NULL);    
}

struct Response* createRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 4)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Account* acc = getAccountByUsername(tokens[0]);
    if (!acc || strcmp(acc->role, "TEACHER"))
        return createResponse(CREATE_ROOM_FAILED, NULL);

    if (getRoomByRoomName(tokens[1]))
        return createResponse(CREATE_ROOM_FAILED, NULL);

    struct Room room;
    strcpy(room.hostName, tokens[0]);
    strcpy(room.roomName, tokens[1]);
    strcpy(room.questionFile, tokens[2]);
    strcpy(room.answerFile, tokens[3]);
    room.status = 0;
    room.numOfStudents = 0;

    saveRoom(room);

    free(acc);
    freeArr(tokens, n);
    return createResponse(OK, NULL);
}

struct Response* dropRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(DROP_ROOM_FAILED, NULL);
    if (strcmp(room->hostName, tokens[0]))
        return createResponse(DROP_ROOM_FAILED, NULL);

    deleteRoom(tokens[1]);

    free(room);
    freeArr(tokens, n);
    return createResponse(OK, NULL);
}

struct Response* listRoom(struct Request* req) {
    struct Room* roomArr = (struct Room*) malloc(sizeof(struct Room)*20);
    int roomArrSize = getAllRooms(roomArr);

    if (roomArrSize < 0) return createResponse(SERVER_ERROR, NULL);
    if (roomArrSize == 0) return createResponse(NO_CONTENT, NULL);

    char data[1000] = "";
    strcat(data, roomArr[0].roomName);
    strcat(data, " ");
    strcat(data, roomArr[0].hostName);
    for (int i = 1; i < roomArrSize; i++) {
        strcat(data, "|");
        strcat(data, roomArr[i].roomName);
        strcat(data, " ");
        strcat(data, roomArr[i].hostName);
    }

    free(roomArr);
    return createResponse(OK, data);
}

struct Response* showRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Room* room = getRoomByRoomName(tokens[0]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);
    
    char data[1000] = "";
    char statusStr[100];
    if (room->status == 0) strcpy(statusStr, "pending");
    else if (room->status == 1) strcpy(statusStr, "started");
    else strcpy(statusStr, "end");

    sprintf(data, "%s|%s|%d|%s", room->roomName, room->hostName, room->numOfStudents, statusStr);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, data);
}

struct Response* joinRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[0]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);

    room->numOfStudents += 1;
    updateRoom(room);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* outRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[0]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);

    room->numOfStudents -= 1;
    updateRoom(room);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* startExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    // tokens[0] : user, tokens[1] : roomName
    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);
    if (strcmp(tokens[0], room->hostName))
        return createResponse(START_FAILED, NULL);
    if (room->status == 1)
        return createResponse(OK, NULL);

    room->status = 1;
    updateRoom(room);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* stopExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    // tokens[0] : user, tokens[1] : roomName
    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);
    if (strcmp(tokens[0], room->hostName))
        return createResponse(STOP_EXAM_FAILED, NULL);
    if (room->status == 2)
        return createResponse(OK, NULL);

    room->status = 2;
    updateRoom(room);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* getExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Room* room = getRoomByRoomName(tokens[0]);
    if (!room)
        return createResponse(ROOM_NOT_FOUND, NULL);
    if (room->status != 1)
        return createResponse(GET_EXAM_FAILED, NULL);

    char* questions[100];
    int n_question = getAllQuestions(room->questionFile, questions);
    
    char data[10000] = "";
    strcat(data, questions[0]);
    for (int i = 1; i < n_question; i++) {
        strcat(data, "|");
        strcat(data, questions[i]);
    }

    freeArr(tokens, n);
    freeArr(questions, n_question);
    free(room);

    return createResponse(OK, data);
}

struct Response* answer(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, "&", tokens);

    if (n != 3)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ANSWER_FAILED, NULL);
    if (room->status == 2)
        return createResponse(ANSWER_FAILED, NULL);
    
    char* userAnswers[100];
    int n_userAnswers = split(tokens[2], "|", userAnswers);

    char* correctAnswers[100];
    int n_correctAnswers = getAllAnswers(room->answerFile, correctAnswers);

    int correctCount = 0;
    for (int i = 0; i < n_correctAnswers; i++) {
        // printf("%s - %s\n", userAnswers[i], correctAnswers[i]);
        if (!strcmp(correctAnswers[i], userAnswers[i]))
            correctCount++;
    }

    char data[1000] = "";
    gcvt(correctCount / n_correctAnswers * 10.0, 2, data);

    freeArr(tokens, n);
    freeArr(userAnswers, n_userAnswers);
    freeArr(correctAnswers, n_correctAnswers);
    free(room);
    return createResponse(OK, data);
}