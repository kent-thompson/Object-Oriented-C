/*
 * encapsulation.h
 */
#ifndef ENCAPSULATION_H_
#define ENCAPSULATION_H_
#include <stdbool.h>
#include <stdio.h>

bool TopLevelOne();
bool TopLevelTwo();

// private functions
bool static printMsgOne();
bool static printMsgTwo();

#endif /* ENCAPSULATION_H_ */
