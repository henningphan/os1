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
  //Append it TODO fixa dummy node, med dummy node är tail aldrig NULL
  if (queue->tail==NULL) {
	printf("Ingen tail\n");
    queue->tail = node;
	queue->head = node; 
  } else {
	queue->tail->next = node;
	queue->tail = node;
  }  
}

/* Pops the head of the queue */
int dequeue(int *extractedValue) {
  //If statement can be removed when dummy is implemented
  struct node* temp = NULL;
  if (queue->head != NULL){
	temp = queue->head;
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
    
  list->head = NULL;
  list->tail = NULL;

  return list;
}

/* Print list */
void print_queue(){
  printf("{ ");
  struct node* node = queue->head;

  while (node != NULL){
    printf("%d, ", node->value);
    node = node->next;
  }
  printf(" }\n");
}




