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

struct LLNode {
    void* value;
    struct LLNode* next;
};

struct LList {
    struct LLNode* head;
    int count;
};


struct LList* initLList();
struct Node* createAccountNode(struct Account);
void addEnd(struct LList* ll, void* value);

List getAllQuestion(char *ques_file);

#endif // __STORAGE_H__
