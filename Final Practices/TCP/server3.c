#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Waiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted\n");
        ssize_t valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread < 0)
        {
            perror("read failed");
            close(new_socket);
            continue;
        }
        buffer[valread] = '\0';
        printf("Received encrypted data: %s\n", buffer);

        decrypt_data(buffer);

        if (send(new_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("send failed");
        }
        else
        {
            printf("Decrypted data sent back to client\n");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
