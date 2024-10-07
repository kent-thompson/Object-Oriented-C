/*
 * encapsulation.c
 */
#include "encapsulation.h"


//bool printMsgOne() {  // could use a positional dependant function, but they go undocumented and can be "lost"
//	printf( "MessageOne\n" );
//	return true;
//}

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
bool static printMsgOne() {
	printf( "MessageOne\n" );
	return true;
}

bool static printMsgTwo() {
	printf( "MessageTwo\n" );
	return true;
}
