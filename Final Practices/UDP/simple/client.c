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
    int sockFd, n;
    char buffer[bufferSize];
    const char *message = "0385\n";
    struct sockaddr_in sevAddr;
    socklen_t len = sizeof(sevAddr);

    if ((sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation at client side is failed\n");
        exit(EXIT_FAILURE);
    }
    memset(&sevAddr, 0, sizeof(sevAddr));

    sevAddr.sin_family = AF_INET;
    sevAddr.sin_addr.s_addr = INADDR_ANY;
    sevAddr.sin_port = htons(port);

    if (sendto(sockFd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&sevAddr, sizeof(sevAddr)) < 0)
    {
        perror("Sending Msg to Server is failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    printf("Message send to server\n");

    n = recvfrom(sockFd, buffer, bufferSize, 0, (struct sockaddr *)&sevAddr, &len);
    if (n < 0)
    {
        perror("Receved from server is failed\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Recvfrom Server: %s\n", buffer);
    return 0;
}
