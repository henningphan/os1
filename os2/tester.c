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

void fill100(void);

int main(int argc, char *argv[] ) {
  struct timeval t1, t2;
  double elapsedTime;

  initialize_queue(); //create a queue
  gettimeofday(&t1, NULL); // Start stopwatch

  // Do things here ---------
  // examples: enqueue(int), dequeue(*int), fill100() and print_queue()
  
  printf("==Add 100\n");
  fill100();
  print_queue();
  printf("==Remove 100\n");
  int *inten;
  int i = 0;
  for(i = 0; i < 100; i++) {
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
void fill100() {
  int i = 1;
  for (i = 1; i <= 100; i++){
	enqueue(i);
  }
}
