/////////////////////////////////////////
// Bailey Wimer
// 04/03/22
// producer.c
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

//The semaphore that will be used as a mutex lock
sem_t *mutex;
//The semaphore that will be used to represent the table of items
sem_t *table;

//Thread to be run by producer
void* thread(void* arg) {
  printf("Producer thread started\n");
  //infinite loop
  for(;;) {
    //wait for turn to enter critical section
    sem_wait(mutex);

    printf("Producer entering...\n");

    printf("Producing...\n");
    //increment table semaphore to emulate producing an item
    sem_post(table);

    printf("Producer exiting\n");
    //leave critical section
    sem_post(mutex);
  }
}


int main(int argc, char** argv) {
  //create a semaphore with proper permissions to act as mutex lock
  mutex = sem_open("/mutex", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
  //create a semaphore with proper permissions to act as the table
  table = sem_open("/table", O_CREAT | O_RDWR, 0644, 1);

  // these shouldn't need to be here, but for some reason even though I specified initializing the semaphores to 1, they always
  // started at 0
  sem_post(table);
  sem_post(mutex);

  //create the thread
  pthread_t t1;
  //define the thread to run the function thread
  pthread_create(&t1, NULL, thread, NULL);
  //start the thread
  pthread_join(t1, NULL);
}
