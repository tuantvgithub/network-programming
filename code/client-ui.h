#ifndef __CLIENT_UI_H__
#define __CLIENT_UI_H__

void homePageScreen(int sockfd);

void loginScreen(int sockfd);
void registerScreen(int sockfd);

void exitGameScreen();
void greetingScreen();

void menuScreen();
void listRoomScreen();
void joinRoomScreen();
void createRoomScreen();
void logoutScreen();

#endif // __CLIENT_UI_H__
