#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sockfd;
    char buffer[MAX];
    char roll_number[10];
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    strcpy(roll_number, "0385");

    sendto(sockfd, roll_number, strlen(roll_number), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Client 2: Roll number sent\n");

    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';

    printf("Client 2: Name received: %s\n", buffer);

    close(sockfd);
    return 0;
}
