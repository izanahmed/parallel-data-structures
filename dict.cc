#include "dict.hh"
#define SIZE 10
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//Hash function
int hash_func(const char * key){
  int val = strlen(key);
  return (val % SIZE);
}
// Initialize a dictionary
void dict_init(my_dict_t* dict) {
  for(int i = 0; i < SIZE; i++){
    dict->nodes[i] = NULL;
    pthread_mutex_init(&(dict->lock[i]), NULL);
  }
}

// Destroy a dictionary
void dict_destroy(my_dict_t* dict) {
  for(int i = 0; i < SIZE; i++){
    pthread_mutex_lock(&dict->lock[i]);
  }

  for(int i = 0; i < SIZE; i++){
  my_node_t* current = dict->nodes[i];
  my_node_t* next;
  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
}
  for(int i = 0; i < SIZE; i++){
    dict->nodes[i] = NULL;
  }
  for(int i = 0; i < SIZE; i++){
    pthread_mutex_unlock(&dict->lock[i]);
  }
}

// Set a value in a dictionary
void dict_set(my_dict_t* dict, const char* key, int value) {
  int index = hash_func(key);
  pthread_mutex_lock(&dict->lock[index]);
  my_node_t * current = dict->nodes[index];

  if (dict_contains(dict, key)){
    while(current != NULL){
      if (strcmp(key, current->key) == 0){
        current->value = value;
        pthread_mutex_unlock(&dict->lock[index]);
        return;
      }
      current = current->next;
    }


  } else {
    my_node_t * new_node = (my_node_t *) malloc(sizeof(my_node_t));
    new_node->key = key;
    new_node->value = value;

    new_node->next = dict->nodes[index];
    dict->nodes[index] = new_node;
    pthread_mutex_unlock(&dict->lock[index]);
    return;
  }
}


// Check if a dictionary contains a key
bool dict_contains(my_dict_t* dict, const char* key) {

  int index = hash_func(key);

  my_node_t * current = dict->nodes[index];

  while(current != NULL){
    if (strcmp(key, current->key) == 0){
      return true;
    }
    current = current->next;
  }

  return false;
}

// Get a value in a dictionary
int dict_get(my_dict_t* dict, const char* key) {
  int index = hash_func(key);
  pthread_mutex_lock(&dict->lock[index]);

  my_node_t * current = dict->nodes[index];

  while(current != NULL){
    if (strcmp(key, current->key) == 0){
      pthread_mutex_unlock(&dict->lock[index]);
      return current->value;
    }
    current = current->next;
  }

  pthread_mutex_unlock(&dict->lock[index]);
  return -1;
}

// Remove a value from a dictionary
void dict_remove(my_dict_t* dict, const char* key) {
  int index = hash_func(key);

  pthread_mutex_lock(&dict->lock[index]);

  my_node_t * current = dict->nodes[index];
  my_node_t * prev = dict->nodes[index];

  while(current != NULL){

    if (strcmp(key, current->key) == 0){
      if (current == dict->nodes[index]){
        dict->nodes[index] = current->next;
        free(current);
        pthread_mutex_unlock(&dict->lock[index]);
        return;
      } else {
        prev->next = current->next;
        free(current);
        pthread_mutex_unlock(&dict->lock[index]);
        return;
      }
    }
    prev = current;
    current = current->next;
  }
}
