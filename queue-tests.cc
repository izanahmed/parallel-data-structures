#include <gtest/gtest.h>

#include "queue.hh"

/****** Queue Invariants ******/

// TODO: Write invariants here. See stack invariants for reference.

/****** Begin Tests ******/

// Basic queue functionality
TEST(QueueTest, BasicQueueOps) {
  my_queue_t q;
  queue_init(&q);

  // Make sure the queue is empty
  ASSERT_TRUE(queue_empty(&q));

  // Make sure taking from the queue returns -1
  ASSERT_EQ(-1, queue_take(&q));

  // Add some items to the queue
  queue_put(&q, 1);
  queue_put(&q, 2);
  queue_put(&q, 3);

  // Make sure the queue is not empty
  ASSERT_FALSE(queue_empty(&q));

  // Take the values from the queue and check them
  ASSERT_EQ(1, queue_take(&q));
  ASSERT_EQ(2, queue_take(&q));
  ASSERT_EQ(3, queue_take(&q));

  // Make sure the queue is empty
  ASSERT_TRUE(queue_empty(&q));

  // Clean up
  queue_destroy(&q);
}

void* thread_func_1(void* val){
  my_queue_t * q = (my_queue_t *) val;

  queue_put(q, 0);
  queue_put(q, 1);
  queue_put(q, 2);
  queue_put(q, 3);
  queue_put(q, 4);
  queue_put(q, 5);

  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);





    return NULL;
}

void* thread_func_2(void* val){
  my_queue_t * q = (my_queue_t *) val;

  queue_put(q, 0);
  queue_put(q, 1);
  queue_put(q, 2);
  queue_put(q, 3);
  queue_put(q, 4);
  queue_put(q, 5);

  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);
  queue_take(q);
  return NULL;
}

TEST(QueueTest, Invariant_1_and_2) {
  // Create a stack
  my_queue_t q;
  queue_init(&q);

  pthread_t t1, t2;

  pthread_create(&t1, NULL, thread_func_1, &q);
  pthread_create(&t2, NULL, thread_func_2, &q);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  ASSERT_TRUE(queue_empty(&q));

  // Clean up
  queue_destroy(&q);
}

void* thread_func_4(void* val){
my_queue_t * q = (my_queue_t *) val;

  queue_put(q, 1);
  queue_put(q, 3);
  queue_put(q, 5);
  queue_put(q, 7);



    return NULL;
}



void* thread_func_3(void* val){
  my_queue_t * q = (my_queue_t *) val;

  queue_put(q, 2);
  queue_put(q, 4);
  queue_put(q, 6);
  queue_put(q, 8);


  return NULL;
}

TEST(QueueTest, Invariant_3_1) {
  // Create a stack
  my_queue_t q;
  queue_init(&q);

  pthread_t t3, t4;

  pthread_create(&t3, NULL, thread_func_3, &q);
  pthread_create(&t4, NULL, thread_func_4, &q);

  pthread_join(t3, NULL);
  pthread_join(t4, NULL);

  int even = 100;
  int odd = 99;

while (!queue_empty(&q)){
  int a = queue_take(&q);
  if (a % 2 == 0){
    if (even == 100){
      even = a;
    } else {
      ASSERT_TRUE(a > even);
      even = a;
    }
  } else {
    if (odd == 99){
      odd = a;
    } else{
      ASSERT_TRUE(a > odd);
      odd = a;
    }
  }
}





  // Clean up
  queue_destroy(&q);
}
int place_holder;

void* thread_func_5(void* val){
my_queue_t * q = (my_queue_t *) val;



int a = queue_take(q);
int b = queue_take(q);

if (b < a){
  place_holder = -1;
}


    return NULL;
}



void* thread_func_6(void* val){
  my_queue_t * q = (my_queue_t *) val;

  int a = queue_take(q);
  int b = queue_take(q);

  if (b < a){
    place_holder = -1;
  }

  return NULL;
}

TEST(QueueTest, Invariant_3_2) {
  // Create a stack
  my_queue_t q;
  queue_init(&q);

  queue_put(&q, 1);
  queue_put(&q, 2);
  queue_put(&q, 3);
  queue_put(&q, 4);
  queue_put(&q, 5);
  queue_put(&q, 6);

  pthread_t t5, t6;

  pthread_create(&t5, NULL, thread_func_5, &q);
  pthread_create(&t6, NULL, thread_func_6, &q);

  ASSERT_FALSE(place_holder == -1);

  pthread_join(t5, NULL);
  pthread_join(t6, NULL);


  // Clean up
  queue_destroy(&q);
}
