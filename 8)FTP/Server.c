#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_TCP_PORT 5035
#define MAX 60

int i, j, tem;
char buff[4096], t;
FILE *f1;

int main() {
    int sockfd, newsockfd, clength;
    struct sockaddr_in serv_addr, cli_addr;
    char t[MAX], str[MAX];
    strcpy(t, "exit");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    printf("\nBinded");
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("\nListening...");
    if (listen(sockfd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    clength = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clength);
    if (newsockfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    ssize_t read_bytes = read(newsockfd, &str, MAX);
    if (read_bytes == -1) {
        perror("Read failed");
        close(newsockfd);
        exit(EXIT_FAILURE);
    }

    printf("\nClient message\n File Name : %s\n", str);

    f1 = fopen(str, "r");
    if (f1 == NULL) {
        perror("File opening failed");
        close(newsockfd);
        exit(EXIT_FAILURE);
    }

    while (fgets(buff, sizeof(buff), f1) != NULL) {
        ssize_t write_bytes = write(newsockfd, buff, MAX);
        if (write_bytes == -1) {
            perror("Write failed");
            fclose(f1);
            close(newsockfd);
            exit(EXIT_FAILURE);
        }
        printf("\n");
    }

    fclose(f1);
    printf("\nFile Transferred\n");
    close(newsockfd);

    return 0;
}