#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define BACKLOG 3

void decrypt_data(char *data)
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        if (data[i] >= 'a' && data[i] <= 'z')
        {
            data[i] -= 3;
        }
        else if (data[i] >= 'A' && data[i] <= 'Z')
        {
            data[i] -= 2;
        }
        else if (data[i] >= '0' && data[i] <= '9')
        {
            data[i] -= 1;
        }
    }
}

int main()
{
    int sockFd, newSockFd;
    struct sockaddr_in servAddr;
    socklen_t servSize = sizeof(servAddr);
    char buffer[BUFFER_SIZE] = {0};

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);

    if (bind(sockFd, (struct sockaddr *)&servAddr, servSize) < 0)
    {
        perror("Server binding failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockFd, BACKLOG) < 0)
    {
        perror("Server listening failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if ((newSockFd = accept(sockFd, (struct sockaddr *)&servAddr, &servSize)) < 0)
    {
        perror("New connection failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    ssize_t valread = read(newSockFd, buffer, BUFFER_SIZE);
    if (valread < 0)
    {
        perror("Read failed");
        close(sockFd);
        close(newSockFd);
        exit(EXIT_FAILURE);
    }
    buffer[valread] = '\0';
    printf("Received encrypted data: %s\n", buffer);
    decrypt_data(buffer);

    if (send(newSockFd, buffer, strlen(buffer), 0) < 0)
    {
        perror("Send failed");
        close(sockFd);
        close(newSockFd);
        exit(EXIT_FAILURE);
    }
    printf("Data sent to client\n");

    close(newSockFd);
    close(sockFd);
    return 0;
}