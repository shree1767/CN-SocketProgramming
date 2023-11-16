#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345); // Choose a port

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    // Accept connection
    newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    if (newSocket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection established with client\n");

    while (1) {
        // Receive message from client
        ssize_t bytesRead = recv(newSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        } else if (bytesRead == 0) {
            printf("Client disconnected\n");
            break;
        }

        printf("Received from client: %.*s\n", (int)bytesRead, buffer);

        // Echo the message back to the client
        if (send(newSocket, buffer, bytesRead, 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
    }

    close(newSocket);
    close(serverSocket);

    return 0;
}