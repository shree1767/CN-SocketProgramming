#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Added for inet_addr
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_TCP_PORT 5035
#define MAX 60

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char send[MAX], recvline[MAX], s[MAX], name[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("\nEnter the source file name : \n");
    if (scanf("%s", send) != 1) {
        perror("Error reading input");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (write(sockfd, send, MAX) == -1) {
        perror("Write failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while ((n = read(sockfd, recvline, MAX)) != 0) {
        printf("%s", recvline);
    }

    close(sockfd);

    return 0;
}