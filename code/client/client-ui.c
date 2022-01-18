#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client-ui.h"
#include "validate.h"
#include "../server/postman.h"
#include "../util/utils.h"

char username[100];
char role[100];

void homePageScreen(int sockfd) {
    int event = 0;

    while (event != 3) {
        printf("__________ Home page __________\n\n");
        printf("-> 1. Login\n");
        printf("-> 2. Register\n");
        printf("-> 3. Exit\n\n");

        printf("--> Your choice: "); 
        scanf("%d", &event);
        while(getchar() != '\n');
        printf("\n");

        switch (event) {
            case 1:
                loginScreen(sockfd);
                break;
            case 2:
                registerScreen(sockfd);
                break;
            case 3:
                exitScreen(sockfd);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void loginScreen(int sockfd) {

    printf("__________ Login Screen __________\n\n");

    char _username[100];
    char _password[100];

    printf("--> username: ");
    scanf("%[^\n]s", _username);
    while(getchar() != '\n');

    printf("--> password: ");
    scanf("%[^\n]s", _password);
    while(getchar() != '\n');

    char message[100];
    strcpy(message, _username);
    strcat(message, " ");
    strcat(message, _password);

    struct Request* req = createRequest(LOGIN, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    strcpy(username, _username);
    strcpy(role, res->data);
    printf("\n-> Success: login successfully.");
    printf("\n-> Notify: hello %s\n\n", username);

    menuScreen(sockfd);
}

void menuScreen(int sockfd) {
    if (!strcmp(role, "TEACHER"))
        teacherMenuScreen(sockfd);
    else if (!strcmp(role, "STUDENT"))
        studentMenuScreen(sockfd);
    else
        printf("\n-> Error: problem displaying function menu.\n\n");
}

void teacherMenuScreen(int sockfd) {
    int event = 0;

    while (event != 2) {
        printf("__________ Teacher Menu Screen __________\n\n");
        
        printf("-> 1. Create room\n");
        printf("-> 2. Logout\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');
        
        switch (event) {
            case 1:
                createRoomScreen(sockfd);
                break;
            case 2:
                logoutScreen(sockfd);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void createRoomScreen(int sockfd) {
    printf("__________ Create room Screen __________\n\n");

    char roomName[100]; 
    int roomNameIsValid = 0;

    while (!roomNameIsValid) {
        printf("--> room name: ");
        scanf("%[^\n]s", roomName);
        while(getchar() != '\n');
        if (validateRoomName(roomName) > 0)
            roomNameIsValid = 1;
        else
            printf("\n-> error: room name is not valid.\n\n");
    }

    char questionFileName[100];
    int questionFileNameIsValid = 0;

    while (!questionFileNameIsValid) {
        printf("--> exam file: ");
        scanf("%[^\n]s", questionFileName);
        while(getchar() != '\n');
        if (validateQuestionFileName(questionFileName) > 0)
            questionFileNameIsValid = 1;
        else
            printf("\n-> error: question file is not valid.\n\n");
    }

    char answerFileName[100];
    int answerFileNameIsValid = 0;

    while (!answerFileNameIsValid) {
        printf("--> answer file: ");
        scanf("%[^\n]s", answerFileName);
        while(getchar() != '\n');
        if (validateQuestionFileName(answerFileName) > 0)
            answerFileNameIsValid = 1;
        else
            printf("\n-> error: answer file is not valid.\n\n");
    }

    char message[1000];
    sprintf(message, "%s %s %s %s", username, roomName, questionFileName, answerFileName);

    struct Request* req = createRequest(CR, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> sucess: create room successfully.\n\n");
    
    teacherRoomScreen(sockfd, roomName);
}

void teacherRoomScreen(int sockfd, char* roomName) {
    int event = 0;

    while (event != 5) {
        printf("__________ Room Screen __________\n\n");

        printf("-> 1. Show room info\n");
        printf("-> 2. Start exam\n");
        printf("-> 3. Stop exam\n");
        printf("-> 4. Get results\n");
        printf("-> 5. Drop room\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');

        switch (event) {
            case 1:
                showRoomInfoScreen(sockfd, roomName);
                break;
            case 2:
                startExam(sockfd, roomName);
                break;
            case 3:
                stopExam(sockfd, roomName);
                break;
            case 4:
                getResults(sockfd, roomName);
                break;
            case 5:
                dropRoomScreen(sockfd, roomName);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void showRoomInfoScreen(int sockfd, char* roomName) {
    printf("__________ Room information Screen __________\n\n");

    char message[100];
    strcpy(message, roomName);
    
    struct Request* req = createRequest(SR, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* roomInfo[20];
    split(res->data, "|", roomInfo);
    
    printf("-> Room name: %s\n", roomInfo[0]);
    printf("-> Host: %s\n", roomInfo[1]);
    printf("-> Number of students: %s\n", roomInfo[2]);
    printf("-> Status: %s\n\n", roomInfo[3]);
}

void startExam(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, username);
    strcat(message, " ");
    strcat(message, roomName);

    struct Request* req = createRequest(START, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Sucess: start exam successfully.\n\n");
}

void stopExam(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, username);
    strcat(message, " ");
    strcat(message, roomName);

    struct Request* req = createRequest(STOP, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Sucess: stop exam successfully.\n\n");
}

void getResults(int sockfd, char* roomName) {
    printf("__________ Result Screen __________\n\n");
    char message[100] = "";
    strcat(message, roomName);

    struct Request* req = createRequest(GET_RESULT, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* results[100];
    int n_result = split(res->data, "|", results);
    
    printf("\t%-10s %-5s\n", "username", "score");
    for (int i = 0; i < n_result; i++) {
        char* tokens[10];
        int n_token = split(results[i], " ", tokens);

        printf("\t%-10s %-5s\n", tokens[0], tokens[1]);
        
        freeArr(tokens, n_token);
    }
    printf("\n");

    freeArr(results, n_result);
}

void dropRoomScreen(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, username);
    strcat(message, " ");
    strcat(message, roomName);

    struct Request* req = createRequest(DR, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: drop room successfully.\n\n");
}

void studentMenuScreen(int sockfd) {
    int event = 0;

    while (event != 3) {
        printf("__________ Student Menu Screen __________\n\n");
        
        printf("-> 1. List room\n");
        printf("-> 2. Join room\n");
        printf("-> 3. Logout\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');
        printf("\n");
        
        switch (event) {
            case 1:
                listRoomScreen(sockfd);
                break;
            case 2:
                joinRoomScreen(sockfd);
                break;
            case 3:
                logoutScreen(sockfd);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void listRoomScreen(int sockfd) {
    printf("__________ List room Screen __________\n\n");
    
    struct Request* req = createRequest(LR, NULL);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* rooms[50];
    int n = split(res->data, "|", rooms);

    for (int i = 0; i < n; i++) {
        char* tokens[20];
        split(rooms[i], " ", tokens);

        printf("-> %d\n", i+1);
        printf("---> Room: %s\n", tokens[0]);
        printf("---> Host: %s\n\n", tokens[1]);
    }
}

void joinRoomScreen(int sockfd) {
    printf("__________ Join room Screen __________\n\n");
    
    char roomName[100];
    printf("-> room name: ");
    scanf("%[^\n]s", roomName);
    while(getchar() != '\n');

    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(JOIN, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: joined the room %s.\n\n", roomName);

    studentRoomScreen(sockfd, roomName);
}

void studentRoomScreen(int sockfd, char* roomName) {
    int event = 0;

    while (event != 3) {
        printf("__________ Room Screen __________\n\n");

        printf("-> 1. Show room info\n");
        printf("-> 2. Get exam\n");
        printf("-> 3. Out room\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');
        printf("\n");
        
        switch (event) {
            case 1:
                showRoomInfoScreen(sockfd, roomName);
                break;
            case 2:
                examScreen(sockfd, roomName);
                break;
            case 3:
                outRoom(sockfd, roomName);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void examScreen(int sockfd, char* roomName) {
    printf("__________ Exam Screen __________\n");
    
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(GET_EXAM, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* questions[100];
    int n = split(res->data, "|", questions);

    char answers[10000] = "";
    strcat(answers, username);
    strcat(answers, "&");
    strcat(answers, roomName);
    strcat(answers, "&");

    for (int i = 0; i < n; i++) {
        char* tokens[100];
        int answer = 0;
        int m = split(questions[i], "#", tokens);

        printf("\n-> Question %d. %s\n", i+1, tokens[0]);
        
        for(int j = 1; j < m; j++) 
            printf("-> %d. %s\n", j, tokens[j]);
        
        printf("\n");
        printf("--> Your answer: ");
        scanf("%d", &answer);
        while(getchar() != '\n');

        if (answer > 0 && answer < m)
            strcat(answers, tokens[answer]);
        else
            strcat(answers, " ");

        if (i + 1 < n) strcat(answers, "|");
    }

    struct Request* req2 = createRequest(ANSWER, answers);
    sendRequest(sockfd, req2);

    struct Response* res2 = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res2);

    if (res2->status != OK) {
        printf("\n-> Failed: %s\n\n", res2->message);
        return;
    }

    printf("\n-> Your score: %s\n\n", res2->data);
}

void outRoom(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(OUT, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: Out room.\n\n");
}

void logoutScreen(int sockfd) {
    char message[100];
    strcpy(message, username);

    struct Request* req = createRequest(LOGOUT, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: logout successfully");
    printf("\n-> Notify: bye %s\n\n", username);
    strcpy(username, "");
}

void registerScreen(int sockfd) {
    printf("__________ Register Screen __________\n\n");

    char _username[100];
    int usernameIsValid = 0;

    while (!usernameIsValid) {
        printf("--> username: ");
        scanf("%[^\n]s", _username);
        while(getchar() != '\n');
        if (validateUsername(_username) > 0)
            usernameIsValid = 1;
        else
            printf("\n-> Error: username is not valid.\n\n");
    }

    char _password[100];
    int passwordIsValid = 0;

    while (!passwordIsValid) {
        printf("--> password: ");
        scanf("%[^\n]s", _password);
        while(getchar() != '\n');
        if (validatePassword(_password) > 0)
            passwordIsValid = 1;
        else
            printf("\n-> Error: password is not valid.\n\n");
    }

    int _roleId = 0;

    while (1) {
        printf("--> role:   1. Student    2. Teacher\n");
        printf("--> Your choice: ");
        scanf("%d", &_roleId);
        while(getchar() != '\n');

        if (_roleId == 1 || _roleId == 2)
            break;
        else
            printf("\n-> Error: role is not valid.\n\n");
    }

    char message[100];
    strcpy(message, _username);
    strcat(message, " ");
    strcat(message, _password);
    strcat(message, " ");
    strcat(message, (_roleId == 1 ? "STUDENT" : "TEACHER"));

    struct Request* req = createRequest(REGISTER, message);
    sendRequest(sockfd, req);

    struct Response* res = (struct Response*) malloc(sizeof(struct Response));;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);        
        return;
    }

    printf("\n-> Success: register successfully.\n\n");
}

void exitScreen(int sockfd) {
    printf("__________ Exit Screen __________\n\n");

    printf("-> Exit\n\n");
}