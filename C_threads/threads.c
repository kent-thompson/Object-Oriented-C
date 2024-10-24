#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

/*
NOTE: Detaching a thread will automatically release resources and "clean up"
    when the thread function ends. No need to call thrd_exit for each function.
SEE: https://github.com/kent-thompson/Cplus-plus Threads.cpp for example of using polling,
    that could be used here as well, for extended usage.
*/

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

    return thrd_success;
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

    return thrd_success;
}

int main() {

    thrd_t threadOne;
    thrd_t threadTwo;
    int rc;
    long id = 1;

    rc = thrd_create( &threadOne, (thrd_start_t)&TaskOne, (void *)id );
    if (rc == thrd_error) {
        printf("ERORR; thrd_create() ONE call failed\n");
        exit(EXIT_FAILURE);
    }
    thrd_detach( threadOne );

    ++id;
    rc = thrd_create( &threadTwo, (thrd_start_t)&TaskTwo, (void *)id );
    if (rc == thrd_error) {
        printf("ERORR; thrd_create() TWO call failed\n");
        exit(EXIT_FAILURE);
    }
    thrd_detach( threadTwo );


    thrd_exit(EXIT_SUCCESS);
}