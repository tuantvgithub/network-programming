#ifndef __STORAGE_H__
#define __STORAGE_H__

#define ACCOUNT_STORAGE_PATH "account.txt"
#define QUESTION_STORAGE_PATH "question.txt"

struct Account {
	char username[45];
	char password[45];
};

int saveAccount(char* username, char* password);
struct Account* getAccountByUsername(char* username);

#endif // __STORAGE_H__
