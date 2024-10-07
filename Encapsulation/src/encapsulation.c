/*
 * encapsulation.c
 */
#include "encapsulation.h"


//bool printMsgOne() {
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

bool static printMsgOne() {
	printf( "MessageOne\n" );
	return true;
}

bool static printMsgTwo() {
	printf( "MessageTwo\n" );
	return true;
}
