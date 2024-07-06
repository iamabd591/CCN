#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char roll_number[MAXLINE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Get roll number from user
    printf("Enter roll number: ");
    fgets(roll_number, MAXLINE, stdin);
    roll_number[strcspn(roll_number, "\n")] = '\0'; // Remove newline character

    // Sending roll number to server
    if (sendto(sockfd, roll_number, strlen(roll_number), MSG_CONFIRM, 
               (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("Message send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Roll number sent\n");

    // Receiving response from server
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, MAXLINE, 0, 
                     (struct sockaddr *)&servaddr, &len);
    if (n == -1) {
        perror("Message receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}
