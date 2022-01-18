#ifndef __SERVER_H__
#define __SERVER_H__


#include "postman.h"

struct Response* handleRequest(int connfd, struct Request *req);

struct Response* login(struct Request* req);
struct Response* logout(struct Request* req);
struct Response* doRegister(struct Request* req);
struct Response* createRoom(struct Request* req);
struct Response* dropRoom(struct Request* req);
struct Response* listRoom(struct Request* req);
struct Response* showRoom(struct Request* req);
struct Response* joinRoom(struct Request* req);
struct Response* outRoom(struct Request* req);
struct Response* startExam(struct Request* req);

#endif // __SERVER_H__
