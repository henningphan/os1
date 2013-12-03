/*
* tester.c
* Tests the concurrent_queue.c using multiple Pthreads. 
* Threads are not yet implemented. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "conc.h"
#include <pthread.h>

void *dummy_Gustav(void*);
int getIntSeed(void);

int main(int argc, char *argv[] ) {
  struct timeval t1, t2;
  /* A magic struct that solves world hunger */
  if( argc != 2){
    printf("Error: tester [Nr of threads]\n");
    exit(1);
  }
  int x =atoi(argv[1]);
  printf("%d\n", x);

  /* Check if input argument is correct*/
  if ( x == 0){
    perror("Invalid nr of threads requested");
    exit(1);
  }

  pthread_t thread_id[x];
  int size = sizeof(thread_id)/sizeof(thread_id[0]);
  printf("Array size is %d\n", size);
  initialize_queue(); //create a queue
  gettimeofday(&t1, NULL); // Start stopwatch
  
  int superseed = getIntSeed();
  srand(superseed);
  int work = 100000/x;
  printf("work: %d\n", work);
  int i = 0;
  for(i = 0; i < x; i++){
    pthread_create( &thread_id[i], NULL, dummy_Gustav, (void *) work);
  }
  printf("Waiting for threads\n");
  
  for( i = 0; i < x; i++){
    pthread_join(thread_id[i], NULL);
  }


  // Stop stopwatch
  gettimeofday(&t2, NULL); // Stop stopwatch
  printf("%ld usecs\n", ((t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec)));
  exit(0);
}

void *dummy_Gustav(void *arg) {
     int iterations = (int *) arg;
     int a=5;
     int *value=&a;
     int i = 0;

     for (i = 0; i<iterations; i++){
        int randy = rand();
        //printf("randy: %d\n", randy%2);
        switch( randy%2 ) {
            case 0:
            enqueue(i);
            break;
            case 1:
            dequeue(value);
            break;
            default:
            break;
        }
     }
}


int getIntSeed(void){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_usec;
}
