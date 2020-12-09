#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define INIT_SEM 0

sem_t sem;

void *threadToWait() {
    sleep(1);

    printf("doing this second ALWAYS\n");
}

void *firstThread() {
    sem_wait(&sem);
    sleep(1);

    printf("doing this first\n");

    sem_post(&sem);
}

int main() {
    sem_init(&sem, 0, INIT_SEM);
    pthread_t first, second;

    printf("Begin main function\n");

    pthread_create(&first, NULL, firstThread, NULL);
    sleep(1);
    pthread_create(&second, NULL, threadToWait, NULL);

    pthread_join(first, NULL);
    pthread_join(second, NULL);

    pthread_exit(NULL);

    printf("Main end\n");

    exit(0);
}