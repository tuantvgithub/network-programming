#ifndef __CLIENT_UI_H__
#define __CLIENT_UI_H__

enum Phase {
    NOT_LOGGED_IN,
    LOGGED_IN,
    
    JOINED_ROOM,

    PLAY_GAME
};

void runCLI(int sockfd);
void displayIntroScreen();


int splitCommand(char* command, char* commandName, char** argv);
int commandIsValid(char* commandName);
void handleCommand(int sockfd, char* command);

void handleCommandNoArgument(int sockfd, char* commandName);
void handleQuitCommand();
void handleLogoutCommand(int sockfd);
void handleLRCommand(int sockfd);
void handleStartCommand(int sockfd);

void handleCommandOneArgument(int sockfd, char* commandName, char** argv);
void handleHelpCommand(char* commandName);
void handleJoinCommand(int sockfd, char** argv);
void handleAnswerCommand(int sockfd, char** argv);

void handleCommandTwoArguments(int sockfd, char* commandName, char** argv);
void handleLoginCommand(int sockfd, char** argv);
void handleRegisterCommand(int sockfd, char** argv);
void handleCRCommand(int sockfd, char** argv);

#endif // __CLIENT_UI_H__
