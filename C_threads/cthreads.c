#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <stdatomic.h>
#include <string.h>

/*
NOTE: Detaching a thread will automatically release resources and "clean up"
    when the thread function ends. No need to call thrd_exit for each function.
    Local Polling is a great way to control and ehnance this technique.
*/


enum state { eError = -1, eNone, eReady, ePaused, eStarted, eRunning, eMilestone, eDone, eComplete };

typedef struct thread_data {
    atomic_ushort id;
    atomic_ulong thrd_id;
	atomic_short state;
	atomic_ulong elapsed;
	char msg[80];       // cannot be atomic
} ThreadData;

mtx_t mutex;            // locking

//------------------------
int64_t getMillis() {
    mtx_lock( &mutex );    
    struct timespec now;
    timespec_get( &now, TIME_UTC );
    int64_t cms = ((int64_t) now.tv_sec) * 1000 + ((int64_t) now.tv_nsec) / 1000000;
    mtx_unlock( &mutex );
    return cms;
}


// display internal (geneally non-atomic) thread data
 void showMsg( ThreadData* pdata ) {
    mtx_lock( &mutex );
    printf( "Msg: %s \n", pdata->msg );
    mtx_unlock( &mutex );
}


//------------------------
int TaskOne( void* pthr_data ) {
    ThreadData* pdata = (ThreadData*)pthr_data;
    pdata->state = eStarted;
    pdata->thrd_id = thrd_current();
    printf( "Hello from Task ONE #%i, thread ID - %lu\n", pdata->id, pdata->thrd_id );

    long begin = getMillis();
    sleep( 2 );     // Do WORK
    long mid = getMillis();
     pdata->elapsed = mid - begin;
    sprintf( pdata->msg, "Thread ONE MID POINT Took %lu Milliseconds to run", pdata->elapsed ); //Notice external data now
    pdata->state = eMilestone;

    sleep( 2 );
    long end = getMillis();
    pdata->elapsed = end - begin;
    printf( "Thread ONE TOTAL Took %lu Milliseconds to run\n", pdata->elapsed );

    pdata->state = eDone;
    return thrd_success;
}


//------------------------
int TaskTwo( void* pthr_data ) {
    ThreadData* pdata = (ThreadData*)pthr_data;
        pdata->state = eStarted;
    pdata->thrd_id = thrd_current();
    printf( "Hello from Task TWO #%i, thread ID - %lu\n", pdata->id, pdata->thrd_id );

    long begin = getMillis();
    sleep( 2 );         // Do WORK
    strcpy( pdata->msg, "Some data inside TaskTwo" );
    showMsg( pdata );   // show externally

    long end = getMillis();
    pdata->elapsed = end - begin;
    printf( "Thread TWO Took %lu Milliseconds to run\n", pdata->elapsed );

    pdata->state = eDone;
    return thrd_success;
}


int main() {

    ThreadData oneData;
    oneData.id = 1;
    oneData.thrd_id = 0;
    oneData.state = eReady;
    oneData.elapsed = 0;

    ThreadData twoData;
    twoData.id = 2;
    twoData.thrd_id = 0;
    twoData.state = eReady;
    twoData.elapsed = 0;

    thrd_t threadOne;
    thrd_t threadTwo;
    int rc;
    int tcounter = 0;   // thread counter = important
    
    rc = thrd_create( &threadOne, (thrd_start_t)&TaskOne, (void*) &oneData );
    if (rc == thrd_error) {
        printf("ERORR; thrd_create() ONE call failed\n");
        exit(EXIT_FAILURE);
    }
    
    rc = thrd_create( &threadTwo, (thrd_start_t)&TaskTwo, (void *) &twoData );
    if (rc == thrd_error) {
        printf("ERORR; thrd_create() TWO call failed\n");
        exit(EXIT_FAILURE);
    }

    thrd_detach( threadOne );
    thrd_detach( threadTwo );

    printf("Loop Started\n");
    bool running = true;
    while( running ) {
        thrd_yield();

        switch( oneData.state ) {
        case eStarted:
            ++tcounter;
            oneData.state = eRunning;
            printf( "TaskOne Running\n" );
            break;

        case eMilestone:
            oneData.state = eRunning;
            showMsg( &oneData );
            break;

        case eDone:
             --tcounter;
            oneData.state = eComplete;
            printf( "TaskOne Done\n" );
            break;
        }

        switch( twoData.state ) {
        case eStarted:
            ++tcounter;
            twoData.state = eRunning;
            printf( "TaskTwo Running\n" );
            break;

        case eMilestone:
            twoData.state = eRunning;
            showMsg( &twoData );
            break;

        case eDone:
             --tcounter;
            twoData.state = eComplete;
            printf( "TaskTwo Done\n" );
            break;
        }
        if( tcounter < 1 ) {
            running = false;
        }
    }
    printf( "Data from outside Task ONE ID: %i Thread ID: %lu Took %lu Milliseconds to run\n", oneData.id, oneData.thrd_id, oneData.elapsed );
    printf( "Data from outside Task TWO ID: %i Thread ID: %lu Took %lu Milliseconds to run\n", twoData.id, twoData.thrd_id, twoData.elapsed );
    printf( "App Complete\n" );
    thrd_exit(EXIT_SUCCESS);
}
