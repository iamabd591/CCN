#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9999
#define BUFFER_SIZE 1024
#define FILES_DIRECTORY "./files/"

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];

    // Step 1: Send client IP address and port number
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    getpeername(client_socket, (struct sockaddr *)&addr, &addr_len);
    char client_info[BUFFER_SIZE];
    sprintf(client_info, "Your IP address: %s, Port: %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    send(client_socket, client_info, strlen(client_info), 0);

    // Step 2: Send list of files available for download
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(FILES_DIRECTORY)) != NULL) {
        char files_list[BUFFER_SIZE];
        memset(files_list, 0, sizeof(files_list));
        while ((ent = readdir(dir)) != NULL) {
            strcat(files_list, ent->d_name);
            strcat(files_list, "\n");
        }
        closedir(dir);
        send(client_socket, files_list, strlen(files_list), 0);
    } else {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Step 3: Receive file name from client
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0)
            break;

        char file_path[BUFFER_SIZE];
        sprintf(file_path, "%s%s", FILES_DIRECTORY, buffer);

        // Step 4: Send content of file to client
        FILE *file = fopen(file_path, "r");
        if (file != NULL) {
            char file_content[BUFFER_SIZE];
            memset(file_content, 0, sizeof(file_content));
            while (fgets(file_content, sizeof(file_content), file) != NULL) {
                send(client_socket, file_content, strlen(file_content), 0);
            }
            fclose(file);
        } else {
            send(client_socket, "File not found. Please enter a valid file name.", strlen("File not found. Please enter a valid file name."), 0);
            continue;
        }

        // Step 5: Ask if client wants to download more files
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_socket, buffer, BUFFER_SIZE, 0) <= 0)
            break;

        if (strcasecmp(buffer, "yes\n") != 0)
            break;
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    // Create a TCP socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to allow reuse of address and port
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        // Accept a new connection
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int *client_socket = malloc(sizeof(int));
        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (*client_socket < 0) {
            perror("Accept failed");
            free(client_socket);
            continue;
        }

        // Create a new thread to handle the client
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, client_socket) != 0) {
            perror("Thread creation failed");
            free(client_socket);
            continue;
        }

        pthread_detach(tid); // Detach thread to avoid memory leak
    }

    close(server_socket);
    return 0;
}