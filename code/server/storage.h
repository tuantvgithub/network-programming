#ifndef __STORAGE_H__
#define __STORAGE_H__

// #define ACCOUNT_STORAGE_PATH "account.txt"
#define ACCOUNT_STORAGE_PATH "./server/account.txt"
#define ROOM_STORAGE_PATH "./server/room.txt"

struct Account {
	char username[45];
	char password[45];
	int connfd;
};

int saveAccount(char* username, char* password);
struct Account* getAccountByUsername(char* username);

struct Question {
	int id;
	char ques[200];
	char choices[4][100];
	char answer[100];
};

int getAllQuestion(char *file_path, struct Question *quesList);
int questionToString(struct Question question, char* buf);

struct Room {
	char roomName[45]; 
	int status;
	char questionsFile[45];
	char hostName[45];
	int numOfPlayer;
	char* players[10];
};
int saveRoom(struct Room room);
struct Room* getRoomByRoomName(char* roomName);
int loadAllRooms(struct Room* roomArr);
int getAllOnRooms(struct Room* roomArr, int size, struct Room* output);

#endif // __STORAGE_H__
