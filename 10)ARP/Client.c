#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For close
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (1) {
        printf("Enter an IP address to query (e.g., 192.168.0.1): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            perror("Error reading input");
            break; // Exit the loop on error
        }

        // Send the IP address query to the server
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        // Receive and display the MAC address response from the server
        memset(buffer, 0, sizeof(buffer));
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL) == -1) {
            perror("Error receiving response");
            break; // Exit the loop on error
        }
        printf("Received MAC address response: %s\n", buffer);
    }

    // Close the socket when done
    close(sockfd);
    return 0;
}