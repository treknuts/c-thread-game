//compile this program: gcc sem_example.c -pthread


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t s;

void *waiting(void *t1)
{
   printf("the waiting thread starts\n");
   sem_wait(&s);
   printf("get a signal from the signaling thread\n");
   pthread_exit(NULL);
}



void *signaling(void *t2)
{
	printf("the signaling thread starts\n");
	sleep(1);
	printf("send a signal to the waiting thread\n");
	sem_post(&s);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   pthread_t thread[2];
   int i;
   sem_init(&s, 0, 0);
   
   pthread_create(&thread[0], NULL, waiting, NULL); 
   pthread_create(&thread[1], NULL, signaling, NULL);


   for (i=0;i<2;i++)
   {
	   pthread_join(thread[i],NULL);
   }

   sem_close(&s);

   pthread_exit(NULL);
}
