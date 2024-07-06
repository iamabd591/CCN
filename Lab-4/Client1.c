// client1.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; ++i) {
        snprintf(buffer, BUFFER_SIZE, "Client 1 message %d", i);
        send(sock, buffer, strlen(buffer), 0);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        buffer[bytes_received] = '\0';
        printf("Client 1 received: %s\n", buffer);
        sleep(1);
    }

    close(sock);
    return 0;
}
