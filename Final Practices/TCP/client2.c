#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void encrypt_data(char *data)
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        if (data[i] >= 'a' && data[i] <= 'z')
        {
            data[i] += 3;
        }
        else if (data[i] >= 'A' && data[i] <= 'Z')
        {
            data[i] += 2;
        }
        else if (data[i] >= '0' && data[i] <= '9')
        {
            data[i] += 1;
        }
    }
}

int main()
{
    int sockFd;
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in servAddr;

    FILE *file = fopen("fileData.txt", "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fread(buffer, sizeof(char), BUFFER_SIZE, file);
    fclose(file);

    encrypt_data(buffer);

    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Client socket creation failed");
        exit(EXIT_FAILURE);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0)
    {
        perror("Invalid address");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Connection failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }

    if (send(sockFd, buffer, strlen(buffer), 0) < 0)
    {
        perror("Failed to send data to server");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    printf("Encrypted message sent to server\n");

    ssize_t valread = read(sockFd, buffer, BUFFER_SIZE);
    if (valread < 0)
    {
        perror("Read failed");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    buffer[valread] = '\0';
    printf("Decrypted data from server: %s\n", buffer);

    close(sockFd);
    return 0;
}
