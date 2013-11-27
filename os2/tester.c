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

void fill100(int, int);
void *dummy_Gustav(void*);

int main(int argc, char *argv[] ) {
  struct timeval t1, t2;
  double elapsedTime;
  if( argc != 2){
    printf("tester [Nr of threads]\n");
    exit(1);
  }
  int x =atoi(argv[1]);
  printf("%d\n", x);
  
  pthread_mutex_t *plist[x];
  initialize_queue(); //create a queue
  gettimeofday(&t1, NULL); // Start stopwatch
  
  int work = 1000/x;
  printf("work: %d\n", work);
  int i = 0;
  for(i = 0; i < x; i++){
    printf("creating a thread\n");
    pthread_t thread;
    pthread_create( &thread, NULL, dummy_Gustav, (void *) work);
    plist[i] = &thread;
  }
  printf("Waiting for threads\n");
  
  for( i = 0; i < x; i++){
    pthread_join(plist[i], NULL);
  }

/*
  pthread_create( &thread1, NULL, dummy_Gustav, (void *) 10000);
  pthread_join(thread1, NULL);

  int *inten;
  int i = 0;
  for(i = 0; i < 200; i++) {
	dequeue(inten);
  }
  */

  // Stop stopwatch
  gettimeofday(&t2, NULL); // Stop stopwatch
  printf("%ld usecs\n", ((t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec)));
  exit(0);
}

/* Put numbers 1-100 in queue*/
void fill100(int x, int iterations) {
  int i = 1;
  for (i = 1; i <= iterations; i++){
	enqueue(x++);
  }
}
void *dummy_Gustav(void *arg) {
//    fill100(1, (int *) arg);

}
