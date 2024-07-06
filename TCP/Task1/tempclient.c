#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER 1024

int main(){
    int sock_fd;
    struct sockaddr_in address;
    char buffer[BUFFER] = {0};
    const char *message = "Hello From CLient";

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("Socket failled");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0){
        perror("Invalid Error");
        exit(EXIT_FAILURE);
    }
    
    if(connect(sock_fd, (struct sockaddr*)&address,sizeof(address))<0){
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    send(sock_fd, message, strlen(message),0);
    printf("Messasge sent to client");
    read(sock_fd, buffer, BUFFER);
    printf("SERVER %s\n", buffer);
    return 0;
}