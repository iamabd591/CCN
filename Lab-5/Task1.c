#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *printinfo(void *argu)
{
    printf("Name: Abdul Hanan Butt\n");
    printf("Registration: L1F20BSCS0385\n");
    return NULL;
}

int main()
{
    pthread_t thread;
    if (pthread_create(&thread, NULL, printinfo, NULL) != 0)
    {
        perror("Error in pthread_create");
        return -1;
    }
    printf("Thread Created\n");

    if (pthread_join(thread, NULL) != 0)
    {
        perror("Error in pthread_join");
        return -1;
    }
    printf("Thread Joined\n");

    return 0;
}
