#include "stack.hh"

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
// Initialize a stack
void stack_init(my_stack_t* stack) {
  pthread_mutex_init(&(stack->lock), NULL);
  stack->head = NULL;
}

// Destroy a stack
void stack_destroy(my_stack_t* stack) {
  pthread_mutex_lock(&stack->lock);
  my_node_t* current = stack->head;
  my_node_t* next;
  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  stack->head = NULL;
  pthread_mutex_unlock(&stack->lock);
}

// Push an element onto a stack
void stack_push(my_stack_t* stack, int element) {
  pthread_mutex_lock(&stack->lock);
  my_node_t *temp;
  temp = (my_node_t*)malloc(sizeof(my_node_t));
  temp->data = element;

  temp->next = stack->head;
  stack->head = temp;
  pthread_mutex_unlock(&stack->lock);
}

// Check if a stack is empty
bool stack_empty(my_stack_t* stack) {
  return (stack->head == NULL);
}

// Pop an element off of a stack
int stack_pop(my_stack_t* stack) {

  pthread_mutex_lock(&stack->lock);

  if(stack->head == NULL){
    pthread_mutex_unlock(&stack->lock);
    return -1;
  }

  my_node_t *temp;
  int data;
  temp = stack->head;
  data = temp->data;
  stack->head = stack->head->next;
  free(temp);
  pthread_mutex_unlock(&stack->lock);
  return data;
}
