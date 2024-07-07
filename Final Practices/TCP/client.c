#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#define port 8080
#define bufferSize 1024

int main()
{
    int sockFd;
    const char *message = "Hello from client\n";
    char buffer[bufferSize];
    struct sockaddr_in sevAddr;
    ssize_t varload;

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Client socket is fialed\n");
        exit(EXIT_FAILURE);
    }
    sevAddr.sin_family = AF_INET;
    sevAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &sevAddr.sin_addr) <= 0)
    {
        perror("Address not supported\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    if (connect(sockFd, (struct sockaddr *)&sevAddr, sizeof(sevAddr)) < 0)
    {
        perror("Connection is failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    if (send(sockFd, message, strlen(message), 0) < 0)
    {
        perror("Sending data is failed\n");
        close(sockFd);
        memset(&sevAddr, 0, sizeof(sevAddr));
        exit(EXIT_FAILURE);
    }
    printf("Message sent to server\n");
    if (varload = read(sockFd, buffer, bufferSize) < 0)
    {
        perror("Receving Failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    printf("Msg Receve from Server: %s\n", buffer);
    close(sockFd);
    return 0;
}