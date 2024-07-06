#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080
#define BUFFER 1024

int main(){

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addreslen = sizeof(address);
    char buffer[BUFFER] = {0};
    const char *message = 'Msg recevied from client';

    if((server_fd = socket(AF_INET, SOCK_STREAM,0))==0){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address,sizeof(address))<0){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd,3)<0){
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    if((new_socket= accept(server_fd,(struct aockaddr*)&address,(socklen_t*)&addreslen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    read(new_socket,buffer,BUFFER);
    printf("Client: %s\n ", buffer);
    send(new_socket, message, strlen(message), 0);
    printf("Msg Send From Server\n");

    return 0;
}