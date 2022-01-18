#ifndef __UTILS_H__
#define __UTILS_H__

#include "../server/storage.h"

int split(char* str, char* delim, char** output);

void freeArr(char** arr, int size);

#endif // __UTILS_H__