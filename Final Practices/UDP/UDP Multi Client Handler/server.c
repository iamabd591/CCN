#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

typedef struct
{
    char roll_number[10];
    char name[50];
} Student;

Student students[] = {
    {"0385", "Hanan"},
    {"0112", "Ali"},
    {"0838", "Huraia"},
    {"0914", "Thair"}};

char *get_student_name(const char *roll_number)
{
    for (int i = 0; i < sizeof(students) / sizeof(students[0]); i++)
    {
        if (strcmp(students[i].roll_number, roll_number) == 0)
        {
            return students[i].name;
        }
    }
    return "Roll number not found";
}

int main()
{
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1)
    {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (char *)buffer, MAX, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        printf("Received roll number from client: %s\n", buffer);

        if (fork() == 0)
        {
            char *name = get_student_name(buffer);
            sendto(sockfd, name, strlen(name), 0, (const struct sockaddr *)&cliaddr, len);
            printf("Sent name to client: %s\n", name);
            exit(0);
        }
    }

    close(sockfd);
    return 0;
}
