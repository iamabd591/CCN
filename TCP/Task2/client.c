#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void encryptData(char *data) {
    while (*data) {
        if (islower(*data)) {
            *data = (*data - 'a' + 3) % 26 + 'a';
        } else if (isupper(*data)) {
            *data = (*data - 'A' + 2) % 26 + 'A';
        } else if (isdigit(*data)) {
            *data = (*data - '0' + 1) % 10 + '0';
        }
        data++;
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Read data from file
    FILE *file = fopen("fileData.txt", "r");
    if (!file) {
        printf("Error: Unable to open file.\n");
        return -1;
    }
    fgets(buffer, BUFFER_SIZE, file);
    fclose(file);

    // Encrypt the data
    encryptData(buffer);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Send encrypted data to server
    send(sock , buffer , strlen(buffer) , 0 );
    printf("Encrypted message sent from client: %s\n", buffer);

    // Wait for response from server
    read( sock , buffer, BUFFER_SIZE);
    printf("Decrypted message received from server: %s\n", buffer);
    
    close(sock);
    return 0;
}
