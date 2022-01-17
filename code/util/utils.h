#ifndef __UTILS_H__
#define __UTILS_H__

int split(char* str, char* delim, char** output);

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
void addToList(struct List* l, void* value);

int accountInList(struct List* l, char* username);
void deleteAccountFromList(struct List* l, char* username);
void deleteRoomFromList(struct List* l, char* roomName);

#endif // __UTILS_H__