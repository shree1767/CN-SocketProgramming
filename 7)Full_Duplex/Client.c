#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Same port as in the server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    while (1) {
        // Read user input
        printf("Enter message: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            perror("Input error");
            exit(EXIT_FAILURE);
        }
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // Send message to server
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Receive response from server
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        printf("Received from server: %.*s\n", (int)bytesRead, buffer);
    }

    close(clientSocket);

    return 0;
}