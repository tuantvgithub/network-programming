#ifndef __STORAGE_H__
#define __STORAGE_H__

#define ACCOUNT_STORAGE_PATH "account.txt"
#define QUESTION_STORAGE_PATH "question.txt"

struct Account {
	char username[45];
	char password[45];
};

struct Question {
	int id;
	char ques[200];
	char choices[4][100];
	char answer[100];
};

struct Room {
	char name[100]; 
};

struct node {
    void *value;
    struct node *next;
};
typedef struct node* Node;

typedef struct list {
    Node head;
    int count;
} List;


Node createNode(void* value);
List newList();
void addEnd(List* l, void* value);

List getAllQuestion(char *ques_file);
List getAllRoom(char *room_file);


int saveAccount(char* username, char* password);
struct Account* getAccountByUsername(char* username);





#endif // __STORAGE_H__
