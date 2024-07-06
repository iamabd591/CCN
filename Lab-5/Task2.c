#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    char name[50];
    char reg_number[100];
} Student_info;

void *printinfo(void *argu)
{
    Student_info *data = (Student_info *)argu;
    printf("Name: %s\n", data->name);
    printf("Registration Number: %s\n", data->reg_number);
    return NULL;
}

int main()
{
    pthread_t thread;
    Student_info Info;

    printf("Enter Your Name: ");
    fgets(Info.name, sizeof(Info.name), stdin);
    Info.name[strcspn(Info.name, "\n")] = 0;

    printf("Enter Your Regitration Number: ");
    fgets(Info.reg_number, sizeof(Info.reg_number), stdin);
    Info.reg_number[strcspn(Info.reg_number, "\n")] = 0;

    if (pthread_create(&thread, NULL, printinfo, &Info) != 0)
    {
        perror("Error in Create_pthread\n");
        return -1;
    }
    printf("Thread Created\n");

    if (pthread_join(thread, NULL))
    {
        perror("Error in pthread_join\n");
        return -1;
    }
    printf("Thread Joined\n");
    return 0;
}