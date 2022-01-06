#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

int saveAccount(char* username, char* password) {
    if (username == NULL || password == NULL) return -1;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "a");

    if (accountStorage == NULL) return -1;
    if (getAccountByUsername(username) != NULL) return -1;

    fprintf(accountStorage, "\n%s %s", username, password);

    fclose(accountStorage);
    return 1;
}

struct Account* getAccountByUsername(char* username) {
    if (username == NULL) return NULL;
    FILE* accountStorage = fopen(ACCOUNT_STORAGE_PATH, "r");
    struct Account* account = (struct Account*) malloc(sizeof(struct Account));
    if (account == NULL) return NULL;

    char _username[45];
    char _password[45];
    while (fscanf(accountStorage, "%s %s", _username, _password) != EOF) {
        if (strcmp(_username, username) == 0) {
            strcpy(account->username, _username);
            strcpy(account->password, _password);
            return account;
        }
    }

    fclose(accountStorage);
    return NULL;
}