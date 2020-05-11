#ifndef DICT_H
#define DICT_H
#define SIZE 10
#include <pthread.h>
#include <stdbool.h>

typedef struct my_node {
  int value;
  const char * key;
  struct my_node * next;
} my_node_t;

typedef struct my_dict {
  pthread_mutex_t lock[SIZE];
  my_node_t * nodes[SIZE];
} my_dict_t;

// Initialize a dictionary
void dict_init(my_dict_t* dict);

// Destroy a dictionary
void dict_destroy(my_dict_t* dict);

// Set a value in a dictionary
void dict_set(my_dict_t* dict, const char* key, int value);

// Check if a dictionary contains a key
bool dict_contains(my_dict_t* dict, const char* key);

// Get a value in a dictionary
int dict_get(my_dict_t* dict, const char* key);

// Remove a value from a dictionary
void dict_remove(my_dict_t* dict, const char* key);

#endif
