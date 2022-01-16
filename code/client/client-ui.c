#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client-ui.h"
#include "validate.h"
#include "../server/postman.h"
#include "../util/utils.h"

char username[100];
char role[100];

// void homePageScreen(int sockfd) {
//     int event = 0;

//     while (event != 3) {
//         printf("__________ Thi trac nghiem __________\n\n");
//         printf("-> 1. Login\n");
//         printf("-> 2. Register\n");
//         printf("-> 3. Exit\n\n");

//         printf("--> Your choice: "); 
//         scanf("%d", &event);
//         while(getchar() != '\n');
//         printf("\n");

//         switch (event) {
//             case 1:
//                 loginScreen(sockfd);
//                 break;
//             case 2:
//                 registerScreen(sockfd);
//                 break;
//             case 3:
//                 exitScreen(sockfd);
//                 break;
//             default:
//                 printf("\n-> Error: your choice is not valid.\n\n");
//         }
//     }
// }

void homePageScreen(int sockfd) {
    struct Request* req;
    struct Response* res;
    // struct Request* req = createRequest(LOGIN, "tuantv tuantv");
    // sendRequest(sockfd, req);
    // struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    // receiveResponse(sockfd, res);

    req = createRequest(LR, NULL);
    sendRequest(sockfd, req);
    res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    req = createRequest(CR, "admin room3 question.abs");
    sendRequest(sockfd, req);
    res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);
}

void loginScreen(int sockfd) {

    printf("__________ Login Screen __________\n\n");

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

    char message[100];
    strcpy(message, _username);
    strcat(message, " ");
    strcat(message, _password);

    struct Request* req = createRequest(LOGIN, message);
    sendRequest(sockfd, req);

    // struct Response* res = NULL;
    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
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
        printf("\n");
        
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
        printf("--> question file: ");
        scanf("%[^\n]s", questionFileName);
        while(getchar() != '\n');
        if (validateQuestionFileName(questionFileName) > 0)
            questionFileNameIsValid = 1;
        else
            printf("\n-> error: question file is not valid.\n\n");
    }

    char message[100];
    strcpy(message, username);
    strcat(message, " ");
    strcat(message, roomName);
    strcat(message, " ");
    strcat(message, questionFileName);

    struct Request* req = createRequest(CR, message);
    sendRequest(sockfd, req);

    // struct Response* res = NULL;
    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> sucess: create room successfully.\n\n");
    
    teacherRoomScreen(sockfd, roomName);
}

void teacherRoomScreen(int sockfd, char* roomName) {
    int event = 0;

    while (event != 3) {
        printf("__________ Room Screen __________\n\n");

        printf("-> 1. Show room info\n");
        printf("-> 2. Start exam\n");
        printf("-> 3. Drop room\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');
        printf("\n");

        switch (event) {
            case 1:
                showRoomInfoScreen(sockfd, roomName);
                break;
            case 2:
                startExamScreen(sockfd, roomName);
                break;
            case 3:
                dropRoomScreen(sockfd, roomName);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void showRoomInfoScreen(int sockfd, char* roomName) {
    printf("__________ Room information Screen __________\n");

    char message[100];
    strcpy(message, roomName);
    
    struct Request* req = createRequest(SR, message);
    sendRequest(sockfd, req);

    // struct Response* res = NULL;
    struct Response* res = (struct Response*) malloc(sizeof(struct Response));
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* roomInfo[20];
    split(res->data, " ", roomInfo);
    
    printf("\n-> Room name: %s\n", roomInfo[0]);
    printf("\n-> Question file: %s\n", roomInfo[1]);
    printf("\n-> Number of students: %s\n", roomInfo[2]);
    // printf("\n-> Number of students submited: %s\n", roomInfo[3]);
}

void startExamScreen(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(START, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }
}

void dropRoomScreen(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(DR, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
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

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    char* rooms[40];
    int n = split(res->data, "|", rooms);

    for (int i = 0; i < n; i++) {
        char* tokens[20];
        int m = split(rooms[i], " ", tokens);
        
        if (m != 3) {
            printf("\n-> Debug: have a problem in listRoomScreen() method.\n\n");
            exit(1);
        }

        printf("-> %d\n", i+1);
        printf("---> Room: %s\n", tokens[0]); 	// room name
        printf("---> Host: %s\n\n", tokens[1]); 	// host name
    }
}

void joinRoomScreen(int sockfd) {
    printf("__________ Join room Screen __________\n\n");
    
    char roomName[100];
    printf("-> room name: ");
    scanf("%[^\n]s", roomName);
    while(getchar() != '\n');

    char message[100];
    strcpy(message, username);
    strcat(message, " ");
    strcat(message, roomName);

    struct Request* req = createRequest(LR, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: join room %s successfully.\n\n", roomName);

    studentRoomScreen(sockfd, roomName);
}

void studentRoomScreen(int sockfd, char* roomName) {
    int event = 0;

    while (event != 2) {
        printf("__________ Room Screen __________\n\n");

        printf("-> 1. Get exam\n");
        printf("-> 2. Out room\n\n");

        printf("--> Your choice: "); scanf("%d", &event);
        while(getchar() != '\n');
        printf("\n");
        
        switch (event) {
            case 1:
                examScreen(sockfd, roomName);
                break;
            case 2:
                outRoom(sockfd, roomName);
                break;
            default:
                printf("\n-> Error: your choice is not valid.\n\n");
        }
    }
}

void examScreen(int sockfd, char* roomName) {
    printf("__________ Exam Screen __________\n\n");
    
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(GET_EXAM, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    // TODO: show questions here

    // TODO: submit all answers here

    // TODO: show result
}

void outRoom(int sockfd, char* roomName) {
    char message[100];
    strcpy(message, roomName);

    struct Request* req = createRequest(OUT, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n-> Success: Out room (OK).\n\n");
}

void logoutScreen(int sockfd) {
    printf("__________ Logout Screen __________\n\n");

    char message[100];
    strcpy(message, username);

    struct Request* req = createRequest(LOGOUT, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
    receiveResponse(sockfd, res);

    if (!res) {
		printf("\n-> Error: can't receive response from server.\n\n");
		return;
    }
    if (res->status != OK) {
        printf("\n-> Failed: %s\n\n", res->message);
        return;
    }

    printf("\n->Success: logout successfully");
    printf("\n->Notify: bye %s\n\n", username);
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

    char message[100];
    strcpy(message, _username);
    strcat(message, " ");
    strcat(message, _password);

    struct Request* req = createRequest(REGISTER, message);
    sendRequest(sockfd, req);

    struct Response* res = NULL;
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