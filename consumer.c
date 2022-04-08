/////////////////////////////////
// Bailey Wimer
// 04/03/22
// consumer.c
// An implementation of the producer/consumer problem using semaphores
//

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>


//The semaphore that is used as a mutex lock
sem_t *mutex;

//The semaphore that represents the table of items being consumed/produced
sem_t *table;

//Thread to be run by consumer
void* thread(void* arg) {
  printf("Consumer thread started\n");
  //infinite loop
  for(;;) {
    //wait to enter critical section until mutex is unlocked
    sem_wait(mutex);
    printf("Consumer entering...\n");

    //attempt to consume from table
    if(sem_trywait(table) == 0) {
      printf("Consuming...\n");
    }

    printf("Consumer exiting\n");
    //exit critical section by unlocking mutex
    sem_post(mutex);
  }
}


int main(int argc, char** argv) {
  //open the shared semaphore "/mutex"
  mutex = sem_open("/mutex", O_RDWR);
  //open the shared semaphore "/table"
  table = sem_open("/table", O_RDWR);

  //define a new thread
  pthread_t t2;
  //create the thread to run the thread function
  pthread_create(&t2, NULL, thread, NULL);
  //start the thread
  pthread_join(t2, NULL);
}
