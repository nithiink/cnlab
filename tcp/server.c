#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int connfd) {
    char buff[MAX];
    int n;
    
    for (;;) {
        bzero(buff, sizeof(buff));
        read(connfd, buff, sizeof(buff));
        printf("From client: %s To client: ", buff);
        bzero(buff, sizeof(buff));
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(connfd, buff, sizeof(buff));
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Server exit.\n");
            break;
        }
    }
}

int main(void) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("socket creation success!\n");
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("bind failed...\n");
        exit(0);
    }
    else
        printf("bind success\n");
    
    if (listen(sockfd, 5) != 0) {
        printf("listen failed\n");
        exit(0);
    }
    else
        printf("listen success\n");
    
    len = sizeof(cli);
    
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Accept failed\n");
        exit(0);
    }
    else
        printf("accept success\n");
    
    func(connfd);
    
    close(connfd);
}
