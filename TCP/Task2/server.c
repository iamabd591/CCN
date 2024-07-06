#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to decrypt the data according to requirements
void decryptData(char *data) {
    while (*data) {
        if (islower(*data)) {
            *data = (*data - 'a' - 3 + 26) % 26 + 'a';
        } else if (isupper(*data)) {
            *data = (*data - 'A' - 2 + 26) % 26 + 'A';
        } else if (isdigit(*data)) {
            *data = (*data - '0' - 1 + 10) % 10 + '0';
        }
        data++;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Receive encrypted data from client
    read( new_socket , buffer, BUFFER_SIZE);
    printf("Encrypted message received from client: %s\n", buffer);

    // Decrypt the data
    decryptData(buffer);

    // Send decrypted data back to client
    send(new_socket , buffer , strlen(buffer) , 0 );
    printf("Decrypted message sent from server: %s\n", buffer);

    close(new_socket);
    close(server_fd);
    return 0;
}
