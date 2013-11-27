/*
* Author: Gustav Ehrenborg, Henning Phan
* concurrent_queue.c
* One lock. 
* Note that the linked list queue is hardcoded and global
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct linked_list* new_linked_list(void);

struct linked_list* queue = NULL;

/* List node, holds a value and a pointer to next */
struct node {
  int value;
  struct node* next;
};

/* A list, keeps track of head and tail */
struct linked_list {
  struct node* head;
  struct node* tail;
  pthread_mutex_t lock;
};

void initialize_queue(void){
  queue = new_linked_list();
  //Set dummy node
}

/* Creates a new node with the given value and appends it to the tail of the queue */
void enqueue(int val) {
  //New node
  struct node* node = (struct node*)malloc(sizeof(struct node));
  node->value = val;
  node->next = NULL;
  pthread_mutex_lock( &queue->lock );
  queue->tail->next = node;
  queue->tail = node;
  pthread_mutex_unlock( &queue->lock );
}

/* Pops the head of the queue */
int dequeue(int *extractedValue) {
  struct node* temp = NULL;
  pthread_mutex_lock( &queue->lock );
  if (queue->head->next != NULL){
	temp = queue->head->next;
	free(queue->head);
	queue->head = temp->next;
	extractedValue = &temp->value;
    pthread_mutex_unlock( &queue->lock );
  } else {
    pthread_mutex_unlock( &queue->lock );
    return 1;
  }
  pthread_mutex_unlock( &queue->lock );
  return 0;
}




/* Creates a new list */
struct linked_list* new_linked_list(){
  struct linked_list* list = (struct linked_list*)malloc(sizeof(struct linked_list));
  struct node* node = (struct node*)malloc(sizeof(struct node));

  /* Init lock */
  pthread_mutex_init( &list->lock, NULL );

  /* Initialize dummy node */
  node->value = 0;
  node->next = NULL;
    
  list->head = node;
  list->tail = node;

  return list;
}

/* Print list */
void print_queue(){
  printf("{ ");
  struct node* node = queue->head->next;

  while (node != NULL){
    printf("%d, ", node->value);
    node = node->next;
  }
  printf(" }\n");
}




