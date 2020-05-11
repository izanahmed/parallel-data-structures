#include <gtest/gtest.h>

#include "stack.hh"
#include <pthread.h>
/****** Stack Invariants ******/

// Invariant 1
// For every value V that has been pushed onto the stack p times and returned by pop q times, there must be p-q copies of this value on the stack. This only holds if p >= q.

// Invariant 2
// No value should ever be returned by pop if it was not first passed to push by some thread.

// Invariant 3
// If a thread pushes value A and then pushes value B, and no other thread pushes these specific values, A must not be popped from the stack before popping B.

/****** Begin Tests ******/

// A simple test of basic stack functionality
TEST(StackTest, BasicStackOps) {
  // Create a stack
  my_stack_t s;
  stack_init(&s);// Push some values onto the stack
  stack_push(&s, 1);
  stack_push(&s, 2);
  stack_push(&s, 3);

  // Make sure the elements come off the stack in the right order
  ASSERT_EQ(3, stack_pop(&s));
  ASSERT_EQ(2, stack_pop(&s));
  ASSERT_EQ(1, stack_pop(&s));

  // Clean up
  stack_destroy(&s);
}

// Another test case
TEST(StackTest, EmptyStack) {
  // Create a stack
  my_stack_t s;
  stack_init(&s);

  // The stack should be empty
  ASSERT_TRUE(stack_empty(&s));

  // Popping an empty stack should return -1
  ASSERT_EQ(-1, stack_pop(&s));

  // Put something on the stack
  stack_push(&s, 0);

  // The stack should not be empty
  ASSERT_FALSE(stack_empty(&s));

  // Pop the element off the stack.
  // We're just testing empty stack behavior, so there's no need to check the resulting value
  stack_pop(&s);

  // The stack should be empty now
  ASSERT_TRUE(stack_empty(&s));

  // Clean up
  stack_destroy(&s);
}

void* thread_func_1(void* val){
  my_stack_t * s = (my_stack_t *) val;

  stack_push(s, 0);
  stack_push(s, 1);
  stack_push(s, 2);
  stack_push(s, 3);
  stack_push(s, 4);
  stack_push(s, 5);

  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);




    return NULL;
}

void* thread_func_2(void* val){
  my_stack_t * s = (my_stack_t *) val;

  stack_push(s, 0);
  stack_push(s, 1);
  stack_push(s, 2);
  stack_push(s, 3);
  stack_push(s, 4);
  stack_push(s, 5);

  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  stack_pop(s);
  return NULL;
}

TEST(StackTest, Invariant_1_and_2) {
  // Create a stack
  my_stack_t s;
  stack_init(&s);

  pthread_t t1, t2;

  pthread_create(&t1, NULL, thread_func_1, &s);
  pthread_create(&t2, NULL, thread_func_2, &s);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  ASSERT_TRUE(stack_empty(&s));

  // Clean up
  stack_destroy(&s);
}



void* thread_func_4(void* val){
my_stack_t * s = (my_stack_t *) val;

  stack_push(s, 1);
  stack_push(s, 3);
  stack_push(s, 5);
  stack_push(s, 7);


    return NULL;
}



void* thread_func_3(void* val){
  my_stack_t * s = (my_stack_t *) val;

  stack_push(s, 2);
  stack_push(s, 4);
  stack_push(s, 6);
  stack_push(s, 8);


  return NULL;
}

TEST(StackTest, Invariant_3_1) {
  // Create a stack
  my_stack_t s;
  stack_init(&s);

  pthread_t t3, t4;

  pthread_create(&t3, NULL, thread_func_3, &s);
  pthread_create(&t4, NULL, thread_func_4, &s);


  int even = 100;
  int odd = 99;

while (!stack_empty(&s)){
  int a = stack_pop(&s);
  if (a % 2 == 0){
    if (even == 100){
      even = a;
    } else{
      ASSERT_TRUE(a < even);
      even = a;
    }
  } else {
    if (odd == 99){
      odd = a;
    } else{
      ASSERT_TRUE(a < odd);
      odd = a;
    }
  }
}


  pthread_join(t3, NULL);
  pthread_join(t4, NULL);


  // Clean up
  stack_destroy(&s);
}

int place_holder;

void* thread_func_5(void* val){
my_stack_t * s = (my_stack_t *) val;



int a = stack_pop(s);
int b = stack_pop(s);

if (b > a){
  place_holder = -1;
}


    return NULL;
}



void* thread_func_6(void* val){
  my_stack_t * s = (my_stack_t *) val;

  int a = stack_pop(s);
  int b = stack_pop(s);

if (b > a){
  place_holder = -1;
}

  return NULL;
}

TEST(StackTest, Invariant_3_2) {
  // Create a stack
  my_stack_t s;
  stack_init(&s);

  stack_push(&s, 1);
  stack_push(&s, 2);
  stack_push(&s, 3);
  stack_push(&s, 4);
  stack_push(&s, 5);
  stack_push(&s, 6);

  pthread_t t5, t6;

  pthread_create(&t5, NULL, thread_func_5, &s);
  pthread_create(&t6, NULL, thread_func_6, &s);

  ASSERT_FALSE(place_holder == -1);

  pthread_join(t5, NULL);
  pthread_join(t6, NULL);


  // Clean up
  stack_destroy(&s);
}
