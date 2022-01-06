#ifndef __CLIENT_UI_H__
#define __CLIENT_UI_H__

void homePageScreen(int sockfd);

void loginScreen(int sockfd);
void registerScreen(int sockfd);

void exitGameScreen();
void greetingScreen();

void menuScreen(int sockfd);
void listRoomScreen(int sockfd);
void joinRoomScreen(int sockfd);
void createRoomScreen(int sockfd);
void logoutScreen();

void startGame(int sockfd);


int parseStringQuestion(char *question, char *ques, char choices[][100]);

#endif // __CLIENT_UI_H__
