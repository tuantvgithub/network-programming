#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "client-ui.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./client IPAddress PortNumber\n"); 
        exit(1);
    }

    int sockfd;

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Error: ");
        exit(1);
    }
        
    struct sockaddr_in servaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Error: ");
       exit(1);
    }

    homePageScreen(sockfd);

	return 0;
}
