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

void fill100(int);
void *dummy_gustav(void*);

int main(int argc, char *argv[] ) {
  struct timeval t1, t2;
  double elapsedTime;
  pthread_t thread1, thread2;

  initialize_queue(); //create a queue
  gettimeofday(&t1, NULL); // Start stopwatch

  // Do things here ---------
  // examples: enqueue(int), dequeue(*int), fill100() and print_queue()
  printf("==Add\n");
  pthread_create( &thread1, NULL, dummy_gustav, (void *) 100);
  pthread_create( &thread2, NULL, dummy_gustav, (void *) 200);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  
  print_queue();
  printf("==Remove\n");
  int *inten;
  int i = 0;
  for(i = 0; i < 200; i++) {
	dequeue(inten);
  }
  print_queue();
  printf("Vinst!\n");


  // Stop doing things--------
  gettimeofday(&t2, NULL); // Stop stopwatch
  printf("%ld usecs\n", ((t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec)));
  exit(0);
}

/* Put numbers 1-100 in queue*/
void fill100(int x) {
  int i = 1;
  for (i = 1; i <= 100; i++){
	enqueue(x++);
  }
}
void *dummy_gustav(void *arg) {
    fill100( (int *) arg);

}
