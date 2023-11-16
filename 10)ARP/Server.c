#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 1024

struct arp_entry {
    char ip_address[16];  // IPv4 address (string representation)
    char mac_address[18]; // MAC address (string representation)
};

struct arp_entry arp_table[] = {
    {"192.168.0.1", "00:11:22:33:44:55"},
    {"192.168.0.2", "AA:BB:CC:DD:EE:FF"},
    // Add more entries as needed
};

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

    printf("ARP Responder is listening on port %d...\n", PORT);

    while (1) {
        // Receive an IP address request from the client
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        printf("Received ARP request: %s\n", buffer);

        // Search for the IP address in the ARP table
        char* mac_address = "Unknown";
        for (size_t i = 0; i < sizeof(arp_table) / sizeof(arp_table[0]); i++) {
            if (strcmp(arp_table[i].ip_address, buffer) == 0) {
                mac_address = arp_table[i].mac_address;
                break;
            }
        }

        // Send the MAC address response back to the client
        sendto(sockfd, mac_address, strlen(mac_address), 0, (struct sockaddr*)&client_addr, client_addr_len);
        printf("Sent ARP response: %s\n", mac_address);
    }

    close(sockfd);
    return 0;
}
