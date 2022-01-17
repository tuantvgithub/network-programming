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

int getAllQuestion(char *file_path, struct Question *quesList);
int questionToString(struct Question question, char* buf);

struct Room {
	char roomName[45]; 
	char hostName[45];
	char questionsFile[45];
	int status;
	int numOfStudents;
};

int saveRoom(struct Room room);
void deleteRoom(char* roomName);
struct Room* getRoomByRoomName(char* roomName);
int getAllRooms(struct Room* roomArr);


#endif // __STORAGE_H__
