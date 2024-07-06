#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    FILE *fileptr;
    char *filename = "example.txt";

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

    printf("Server is listening on port %d...\n", PORT);

    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connected: client socket %d, pid %d\n", new_socket, getpid());

        // Receive file name from client
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Requested file: %s\n", buffer);

        // Open and send file to client
        fileptr = fopen(filename, "r");
        if (fileptr == NULL) {
            perror("File opening error");
            exit(EXIT_FAILURE);
        }

        while (fgets(buffer, BUFFER_SIZE, fileptr) != NULL) {
            send(new_socket, buffer, strlen(buffer), 0);
        }

        fclose(fileptr);
        close(new_socket);
        printf("File sent to client. Connection closed.\n");
    }

    return 0;
}
