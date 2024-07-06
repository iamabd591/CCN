#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define port 8080
#define bufferSize 1024

int main()
{
    int sockFd;
    char buffer[bufferSize];
    struct sockaddr_in sevAddr, cliAddr;
    socklen_t len = sizeof(cliAddr);
    const char *message = "Abdul Hanan\n";

    if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Server side socket creation is failed\n");
        exit(EXIT_FAILURE);
    }

    memset(&sevAddr, 0, sizeof(sevAddr));
    memset(&cliAddr, 0, sizeof(cliAddr));

    sevAddr.sin_family = AF_INET;
    sevAddr.sin_addr.s_addr = INADDR_ANY;
    sevAddr.sin_port = htons(port);

    if (bind(sockFd, (const struct sockaddr *)&sevAddr, sizeof(sevAddr)) < 0)
    {
        perror("Socket binding is failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    int n = recvfrom(sockFd, buffer, bufferSize, 0, (struct sockaddr *)&cliAddr, &len);
    if (n == -1)
    {
        perror("Receve from client is failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Recvfrom Client: %s\n", buffer);

    if (sendto(sockFd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&cliAddr, len) < 0)
    {
        perror("Sending Message to client failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    printf("Message send to client\n");
    close(sockFd);
    return 0;
}