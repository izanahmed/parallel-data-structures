#include "queue.hh"
#include "pthread.h"
#include <stdlib.h>

// Initialize a new queue
void queue_init(my_queue_t* queue) {
  pthread_mutex_init(&(queue->front_lock),NULL);
  pthread_mutex_init(&(queue->end_lock),NULL);
  queue->front = NULL;
  queue->end = NULL;
}

// Destroy a queue
void queue_destroy(my_queue_t* queue) {
  pthread_mutex_lock(&queue->front_lock);
  pthread_mutex_lock(&queue->end_lock);
  // if queue is empty, return
  if (queue_empty(queue)){
     pthread_mutex_unlock(&queue->front_lock);
     pthread_mutex_unlock(&queue->end_lock);
    return;
  }

  //destroy the current node and remember the next pointer by curr
  struct Node* toDestroy = queue->front;
  struct Node*  curr = toDestroy->next;
  while (curr != NULL) {
    free(toDestroy);
    toDestroy = curr;
    curr = curr->next;
  }

  // free the pointer to last node
  free(toDestroy);
   pthread_mutex_unlock(&queue->front_lock);
   pthread_mutex_unlock(&queue->end_lock);
}

// Put an element at the end of a queue
void queue_put(my_queue_t* queue, int element) {
  struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
  newNode->key = element;
  newNode->next = NULL;
  pthread_mutex_lock(&queue->end_lock);
  if (queue_empty(queue)){
    queue->front = newNode;
    queue->end = newNode;
    pthread_mutex_unlock(&queue->end_lock);
    return;
  }
  queue->end->next = newNode;
  queue->end = newNode;
  pthread_mutex_unlock(&queue->end_lock);
}

// Check if a queue is empty
bool queue_empty(my_queue_t* queue) {
  return ((queue->front == NULL) && (queue->end == NULL));
}

// Take an element off the front of a queue
int queue_take(my_queue_t* queue){
  pthread_mutex_lock(&queue->front_lock);
  if (queue_empty(queue)){
      pthread_mutex_unlock(&queue->front_lock);
    return -1;
  }
  struct Node* temp = queue->front;
  int res = temp->key;
  queue->front = queue->front->next;

  if (queue->front == NULL) {
    queue->end = NULL;
  }

  free(temp);
  pthread_mutex_unlock(&queue->front_lock);
  return res;
}
