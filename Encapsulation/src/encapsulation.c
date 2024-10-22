/*
 * encapsulation.c
 */
#include "encapsulation.h"

//bool printMsgOne() {  // could use a positional dependant function, but they go undocumented and can be "lost"
//	printf( "MessageOne\n" );
//	return true;
//}

// private functions
static bool printMsgOne();
static bool printMsgTwo();


bool TopLevelOne() {
	printf( "IopLevelOne\n" );
	printMsgOne();
	return true;
}

bool TopLevelTwo() {
	printf( "IopLevelTwo\n" );
	printMsgTwo();
	return true;
}

// private functions
static bool printMsgOne() {
	printf( "MessageOne\n" );
	return true;
}

static bool printMsgTwo() {
	printf( "MessageTwo\n" );
	return true;
}
