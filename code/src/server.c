#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../lib/postman.h"
#include "../lib/storage.h"
#include "../lib/utils.h"
#include "../lib/server.h"

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
                printf("Request:\n  -opcode: %s\n  -message: %s\n", readOpcode(req->opcode), req->message);

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
        case STOP:
            return stopExam(request);
        case GET_EXAM:
            return getExam(request);
        case GET_RESULT:
            return getResult(request);
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
    if (!account)
        return createResponse(USERNAME_NOT_EXISTS, NULL);

    if (strcmp(account->password, tokens[1]))
        return createResponse(INCORRECT_PASSWORD, NULL);

    if (accountIsActive(tokens[0]) > 0)
        return createResponse(ACCOUNT_ALREADY_LOGGED_IN, NULL);

    if (saveActiveAccount(tokens[0]) < 0)
        return createResponse(SERVER_ERROR, NULL);

    char data[1000];
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

    if (deleteActiveAccount(tokens[0]) < 0)
        return createResponse(SERVER_ERROR, NULL);

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
        return createResponse(USERNAME_ALREADY_EXISTS, NULL);
    
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
    if (!acc)
        return createResponse(USERNAME_NOT_EXISTS, NULL);
    
    if (strcmp(acc->role, "TEACHER"))
        return createResponse(NOT_ALLOWED, NULL);

    if (getRoomByRoomName(tokens[1]))
        return createResponse(ROOM_ALREADY_EXISTS, NULL);

    struct Room room;
    strcpy(room.hostName, tokens[0]);
    strcpy(room.roomName, tokens[1]);
    strcpy(room.questionFile, tokens[2]);
    strcpy(room.answerFile, tokens[3]);
    room.status = 0;
    room.numOfStudents = 0;

    if (saveRoom(room) < 0)
        return createResponse(SERVER_ERROR, NULL);

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
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (strcmp(room->hostName, tokens[0]))
        return createResponse(NOT_ALLOWED, NULL);

    if (deleteRoom(tokens[1]) < 0)
        return createResponse(SERVER_ERROR, NULL);

    free(room);
    freeArr(tokens, n);
    return createResponse(OK, NULL);
}

struct Response* listRoom(struct Request* req) {
    struct Room* roomArr = (struct Room*) malloc(sizeof(struct Room)*20);
    int roomArrSize = getAllRooms(roomArr);

    if (roomArrSize < 0) return createResponse(SERVER_ERROR, NULL);
    if (roomArrSize == 0) return createResponse(OK, NULL);

    char data[1000] = "";
    sprintf(data, "%s %s", roomArr[0].roomName, roomArr[0].hostName);
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
        return createResponse(ROOM_NOT_EXISTS, NULL);
    
    char data[1000] = "";
    char statusStr[100];
    if (room->status == 0) strcpy(statusStr, "pending");
    else if (room->status == 1) strcpy(statusStr, "started");
    else strcpy(statusStr, "finished");

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
        return createResponse(ROOM_NOT_EXISTS, NULL);

    room->numOfStudents += 1;
    if (updateRoom(room) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* outRoom(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 1)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(OK, NULL);

    room->numOfStudents -= 1;
    if (updateRoom(room) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* startExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (strcmp(tokens[0], room->hostName))
        return createResponse(NOT_ALLOWED, NULL);

    if (room->status == 1)
        return createResponse(OK, NULL);

    room->status = 1;
    if (updateRoom(room) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* stopExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);
    
    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (strcmp(tokens[0], room->hostName))
        return createResponse(NOT_ALLOWED, NULL);

    if (room->status == 2)
        return createResponse(OK, NULL);

    room->status = 2;
    if (updateRoom(room) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    free(room);
    return createResponse(OK, NULL);
}

struct Response* getExam(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);

    if (isSubmited(tokens[1], tokens[0]) > 0)
        return createResponse(NOT_ALLOWED, NULL);

    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (room->status == 0)
        return createResponse(EXAM_NOT_STARTED, NULL);
    if (room->status == 2)
        return createResponse(EXAM_ALREADY_FINISHED, NULL);

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

struct Response* getResult(struct Request* req) {
    char* tokens[5];
    int n = split(req->message, " ", tokens);

    if (n != 2)
        return createResponse(SYNTAX_ERROR, NULL);

    struct Room* room = getRoomByRoomName(tokens[1]);
    if (!room)
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (strcmp(tokens[0], room->hostName))
        return createResponse(NOT_ALLOWED, NULL);

    char* results[100];
    int n_result = getAllResult(tokens[1], results);
    
    if (n_result < 0)
        return createResponse(SERVER_ERROR, NULL);
    if (n_result == 0)
        return createResponse(OK, NULL);

    char data[10000] = "";
    strcat(data, results[0]);
    for (int i = 1; i < n_result; i++) {
        strcat(data, "|");
        strcat(data, results[i]);
    }

    freeArr(tokens, n);
    freeArr(results, n_result);
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
        return createResponse(ROOM_NOT_EXISTS, NULL);

    if (room->status == 2)
        return createResponse(EXAM_ALREADY_FINISHED, NULL);
    if (room->status == 0)
        return createResponse(EXAM_NOT_STARTED, NULL);
    
    char* userAnswers[100];
    int n_userAnswers = split(tokens[2], "|", userAnswers);

    char* correctAnswers[100];
    int n_correctAnswers = getAllAnswers(room->answerFile, correctAnswers);

    int correctCount = 0;
    for (int i = 0; i < n_correctAnswers; i++) {
        if (!strcmp(correctAnswers[i], userAnswers[i]))
            correctCount++;
    }

    char data[1000] = "";
    float score = (float) correctCount / n_correctAnswers * 10;
    gcvt(score, 2, data);

    if (saveResult(room->roomName, tokens[0], data) < 0)
        return createResponse(SERVER_ERROR, NULL);

    freeArr(tokens, n);
    freeArr(userAnswers, n_userAnswers);
    freeArr(correctAnswers, n_correctAnswers);
    free(room);

    return createResponse(OK, data);
}