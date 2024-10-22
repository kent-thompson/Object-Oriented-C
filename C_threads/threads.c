#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>


//------------------------
int64_t getMillis() {
    struct timespec now;
    timespec_get( &now, TIME_UTC );
    return ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
}


//------------------------
int TaskOne( void* thr_id ) {
  long tid = (long)thr_id;
  printf( "Hello from thread One #%ld, thread ID - %lu\n", tid, thrd_current() );

long begin = getMillis();
sleep( 3 );
long end = getMillis();
long elapsed = end - begin;
 printf( "Thread ONE Took %lu Milliseconds to run\n", elapsed );
  
  thrd_exit(EXIT_SUCCESS);
}


//------------------------
int TaskTwo(void *thr_id) {
  long tid = (long)thr_id;
  printf( "Hello from thread Two #%ld, thread ID - %lu\n", tid, thrd_current() );

long begin = getMillis();
sleep( 2 );
long end = getMillis();
long elapsed = end - begin;
 printf( "Thread TWO Took %lu Milliseconds to run\n", elapsed );

  thrd_exit(EXIT_SUCCESS);
}

int main() {

    thrd_t threadOne;
    thrd_t threadTwo;
    int rc;
    long id = 1;

    rc = thrd_create( &threadOne, (thrd_start_t)&TaskOne, (void *)id );
    if (rc == thrd_error) {
      printf("ERORR; thrd_create() call failed\n");
      exit(EXIT_FAILURE);
    }

    id = 2;
    rc = thrd_create( &threadTwo, (thrd_start_t)&TaskTwo, (void *)id );
    if (rc == thrd_error) {
      printf("ERORR; thrd_create() call failed\n");
      exit(EXIT_FAILURE);
    }

  thrd_exit(EXIT_SUCCESS);
}