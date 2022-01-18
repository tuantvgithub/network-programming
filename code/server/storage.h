#ifndef __STORAGE_H__
#define __STORAGE_H__

#define ACCOUNT_STORAGE_PATH "./server/account.txt"
#define ACTIVE_ACCOUNT_STORAGE_PATH "./server/active_account.txt"
#define ACTIVE_ACCOUNT_STORAGE_TMP_PATH "./server/active_account_tmp.txt"

#define ROOM_STORAGE_PATH "./server/room.txt"
#define ROOM_STORAGE_TMP_PATH "./server/room_tmp.txt"


struct Account {
	char username[45];
	char password[45];
	char role[45];
};

int saveAccount(char* username, char* password, char* role);
struct Account* getAccountByUsername(char* username);

int accountIsActive(char* username);
int saveActiveAccount(char* username);
int deleteActiveAccount(char* username);

struct Question {
	int id;
	char ques[200];
	char choices[4][100];
	char answer[100];
};

int getAllQuestions(char *file_path, char** output);
int getAllAnswers(char* file_path, char** output);

struct Room {
	char roomName[45]; 
	char hostName[45];
	char questionFile[45];
	char answerFile[45];
	int status;
	int numOfStudents;
};

int saveRoom(struct Room room);
void deleteRoom(char* roomName);
void updateRoom(struct Room* room);
struct Room* getRoomByRoomName(char* roomName);
int getAllRooms(struct Room* roomArr);

void saveResult(char* roomName, char* username, char* score);
int getAllResult(char* roomName, char** output);

#endif // __STORAGE_H__
