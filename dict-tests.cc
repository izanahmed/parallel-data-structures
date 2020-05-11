#include <gtest/gtest.h>

#include "dict.hh"
#include <pthread.h>

/****** Dictionary Invariants ******/

/*
 1. For value, key pair "A" set in dictionary, and then removed from the dictionary, the dictionary should not contain that value, key pair "A".
 2. Every key, value pair set in the dictionary, the dictionary should contain it.

*/

/****** Synchronization ******/

// Under what circumstances can accesses to your dictionary structure can proceed in parallel? Answer below.
// Set function can proceed in parallel.


// When will two accesses to your dictionary structure be ordered by synchronization? Answer below.
// Contain function will cause the dictionary structure to be ordered by synchronization.
//
//

/****** Begin Tests ******/

// Basic functionality for the dictionary
TEST(DictionaryTest, BasicDictionaryOps) {
  my_dict_t d;
  dict_init(&d);

  // Make sure the dictionary does not contain keys A, B, and C
  ASSERT_FALSE(dict_contains(&d, "A"));
  ASSERT_FALSE(dict_contains(&d, "B"));
  ASSERT_FALSE(dict_contains(&d, "C"));

  // Add some values
  dict_set(&d, "A", 1);
  dict_set(&d, "B", 2);
  dict_set(&d, "C", 3);

  // Make sure these values are contained in the dictionary
  ASSERT_TRUE(dict_contains(&d, "A"));
  ASSERT_TRUE(dict_contains(&d, "B"));
  ASSERT_TRUE(dict_contains(&d, "C"));

  // Make sure these values are in the dictionary
  ASSERT_EQ(1, dict_get(&d, "A"));
  ASSERT_EQ(2, dict_get(&d, "B"));
  ASSERT_EQ(3, dict_get(&d, "C"));

  // Set some new values
  dict_set(&d, "A", 10);
  dict_set(&d, "B", 20);
  dict_set(&d, "C", 30);

  // Make sure these values are contained in the dictionary
  ASSERT_TRUE(dict_contains(&d, "A"));
  ASSERT_TRUE(dict_contains(&d, "B"));
  ASSERT_TRUE(dict_contains(&d, "C"));

  // Make sure the new values are in the dictionary
  ASSERT_EQ(10, dict_get(&d, "A"));
  ASSERT_EQ(20, dict_get(&d, "B"));
  ASSERT_EQ(30, dict_get(&d, "C"));

  // Remove the values
  dict_remove(&d, "A");
  dict_remove(&d, "B");
  dict_remove(&d, "C");

  // Make sure these values are not contained in the dictionary
  ASSERT_FALSE(dict_contains(&d, "A"));
  ASSERT_FALSE(dict_contains(&d, "B"));
  ASSERT_FALSE(dict_contains(&d, "C"));

  // Make sure we get -1 for each value
  ASSERT_EQ(-1, dict_get(&d, "A"));
  ASSERT_EQ(-1, dict_get(&d, "B"));
  ASSERT_EQ(-1, dict_get(&d, "C"));

  // Clean up
  dict_destroy(&d);
}

void* thread_func_1(void* val){
  my_dict_t * d = (my_dict_t *) val;

  dict_set(d, "Ho", 1);
  dict_set(d, "Hey", 2);
  dict_set(d, "Hi", 3);
  dict_set(d, "fooo", 4);
  return NULL;

}

void* thread_func_2(void* val){
  return NULL;
}

TEST(DictionaryTest, Invariant_1) {
  my_dict_t d;
  dict_init(&d);

  pthread_t t1, t2;

  pthread_create(&t1, NULL, thread_func_1, &d);
  pthread_create(&t2, NULL, thread_func_2, &d);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  dict_remove(&d, "Hey");
  dict_remove(&d, "Hi");
  ASSERT_FALSE(dict_contains(&d, "Hey"));
  ASSERT_FALSE(dict_contains(&d, "Hi"));
  ASSERT_TRUE(dict_contains(&d, "fooo"));
  ASSERT_TRUE(dict_contains(&d, "Ho"));

  // Clean up
  dict_destroy(&d);
}

TEST(DictionaryTest, Invariant_2){
  my_dict_t d;
  dict_init(&d);

  dict_set(&d, "Hello", 1);
  dict_set(&d, "whatsup", 5);
  dict_set(&d, "ew", 7);

  ASSERT_TRUE(dict_contains(&d, "Hello"));
  ASSERT_TRUE(dict_contains(&d, "whatsup"));
  ASSERT_TRUE(dict_contains(&d, "ew"));

  dict_destroy(&d);
}
