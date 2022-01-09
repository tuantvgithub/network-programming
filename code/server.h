#ifndef __SERVER_H__
#define __SERVER_H__


#include "postman.h"

struct Response* handleRequest(struct Request *req);

struct Response* login(struct Request* req);
struct Response* logout(struct Request* req);
struct Response* doRegister(struct Request* req);
struct Response* listRoom(struct Request* req);

#endif // __SERVER_H__
