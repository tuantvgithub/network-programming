#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "client-ui.h"

int createConnection(char* svIP, char* port);
extern int sockfd;
int main(int argc, char** argv) {
    if (argc != 3) {
        perror("Usage: IPadd Port"); 
        exit(1);
    }
    sockfd = createConnection(argv[1], argv[2]);
	homePageScreen();
	return 0;
}


int createConnection(char* svIP, char* port) {
    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        return 0;
    }
        
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(svIP);
    servaddr.sin_port =  htons(atoi(port));

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        return -1;
    }

    return sockfd;
}
