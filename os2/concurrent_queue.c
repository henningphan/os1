/*
* concurrent_queue.c
* One lock. 
* Note that the linked list queue is hardcoded and global
*/

#include <stdio.h>
#include <stdlib.h>

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
  queue->tail->next = node;
  queue->tail = node;
}

/* Pops the head of the queue */
int dequeue(int *extractedValue) {
  struct node* temp = NULL;
  if (queue->head->next != NULL){
	temp = queue->head->next;
	queue->head = temp->next;
	extractedValue = &temp->value;
	free(temp);
  } else {
    return 1;
  }
  return 0;
}




/* Creates a new list */
struct linked_list* new_linked_list(){
  struct linked_list* list = (struct linked_list*)malloc(sizeof(struct linked_list));
  struct node* node = (struct node*)malloc(sizeof(struct node));

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




