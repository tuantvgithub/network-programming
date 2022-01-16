#ifndef __STORAGE_H__
#define __STORAGE_H__

#define ACCOUNT_STORAGE_PATH "account.txt"
#define ROOM_STORAGE_PATH "room.txt"

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

struct List* getAllQuestion(char* quesFile);


struct Room {
	char roomName[45]; 
	int status;
	char questionsFile[45];
	char hostName[45];
	int numOfPlayer;
	char* players[10];
};

int loadAllRooms(struct Room* roomArr);
int getAllOnRooms(struct Room* roomArr, int size, struct Room* output);


struct Node {
	void* value;
	struct Node* next;
};

struct List {
	struct Node* head;
	int count;
};

struct Node* createNode(void* value);
struct List* newList();
void addEnd(struct List* l, void* value);

#endif // __STORAGE_H__