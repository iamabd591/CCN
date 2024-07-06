#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

// Hardcoded roll number to name mapping
const char *get_name_from_roll_number(const char *roll_number) {
    if (strcmp(roll_number, "101") == 0) {
        return "Alice";
    } else if (strcmp(roll_number, "102") == 0) {
        return "Bob";
    } else if (strcmp(roll_number, "103") == 0) {
        return "Charlie";
    } else {
        return "Unknown Roll Number";
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(cliaddr);

    while (1) {
        // Receive roll number from client
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n == -1) {
            perror("Receive failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[n] = '\0';
        printf("Received roll number: %s\n", buffer);

        // Get name corresponding to roll number
        const char *name = get_name_from_roll_number(buffer);

        // Send name to client
        if (sendto(sockfd, name, strlen(name), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) == -1) {
            perror("Send failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Sent name: %s\n", name);
    }

    close(sockfd);
    return 0;
}
