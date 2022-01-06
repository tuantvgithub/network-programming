#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "postman.h"

#define MAXLINE 4096 /*max text line length*/
#define LISTENQ 8 /*maximum number of client connections*/

int main (int argc, char **argv) {
    if (argc != 2) {
        perror("Usage: IPadd"); 
        return -1;
    }

    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in servaddr;

    //Create a socket for the soclet
    //If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        return -1;
    }

    //preparation of the socket address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    //bind the socket
    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen (listenfd, LISTENQ);

    printf("%s\n","Server started!");

    struct sockaddr_in clientAddr;
    int rcvBytes, sendBytes, clientAddrLen = sizeof(clientAddr);
    char buff[MAXLINE + 1];

    connfd = accept(listenfd, (struct sockaddr *) &clientAddr, &clientAddrLen);
	while(1){
        rcvBytes = recv(connfd, buff, MAXLINE, 0);
        if (rcvBytes < 0)
            perror("Error: ");
        if (rcvBytes > 0) {
            buff[rcvBytes] = '\0';
            printf("Receive from client[%d:%d] %s\n",
                    inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buff);
        }
    }

}