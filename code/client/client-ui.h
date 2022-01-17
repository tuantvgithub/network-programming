#ifndef __CLIENT_UI_H__
#define __CLIENT_UI_H__

void homePageScreen(int scokfd);
void loginScreen(int sockfd);
void registerScreen(int sockfd);
void exitScreen(int sockfd);

void menuScreen(int sockfd);
void logoutScreen(int sockfd);

void teacherMenuScreen(int sockfd);
void createRoomScreen(int sockfd);
void teacherRoomScreen(int sockfd, char* roomName);
void showRoomInfoScreen(int sockfd, char* roomName);
void startExamScreen(int sockfd, char* roomName);
void getResults(int sockfd, char* roomName);
void dropRoomScreen(int sockfd, char* roomName);

void studentMenuScreen(int sockfd);
void listRoomScreen(int sockfd);
void joinRoomScreen(int sockfd);
void studentRoomScreen(int sockfd, char* roomName);
void examScreen(int sockfd, char* roomName);
void outRoom(int sockfd, char* roomName);

#endif // __CLIENT_UI_H__