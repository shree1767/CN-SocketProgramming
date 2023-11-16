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

    // Send message to server
    char message[] = "Hello, server!";
    if (send(clientSocket, message, sizeof(message), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    printf("Sent to server: %s\n", message);

    close(clientSocket);

    return 0;
}