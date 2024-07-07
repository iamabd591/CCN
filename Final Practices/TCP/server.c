#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 3

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockFd, newSockFd;
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in servAddr;
    socklen_t servAddrLen = sizeof(servAddr);
    const char *message = "Hello from server\n";

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error("Server socket creation failed");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    if (bind(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        error("Binding server failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockFd, BACKLOG) < 0)
    {
        error("Listening failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if ((newSockFd = accept(sockFd, (struct sockaddr *)&servAddr, &servAddrLen)) < 0)
    {
        error("Accepting connection failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    ssize_t valread = read(newSockFd, buffer, BUFFER_SIZE);
    if (valread < 0)
    {
        error("Read failed");
        close(sockFd);
        close(newSockFd);
        exit(EXIT_FAILURE);
    }
    printf("Server Received: %s\n", buffer);

    if (send(newSockFd, message, strlen(message), 0) < 0)
    {
        error("Sending message failed");
        close(sockFd);
        close(newSockFd);
        exit(EXIT_FAILURE);
    }
    printf("Message Sent\n");

    close(newSockFd);
    close(sockFd);
    return 0;
}
