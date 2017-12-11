#ifndef HELPER_H
#define HELPER_H

#include "constants.h"

#include <vector>

/* The function, given number vector, checks if it has a duplicate element */
bool hasDuplicateElement(std::vector<int> numberVector, int number);

/* Converts any character to integer so that i.e. "A" has a value of "0",
"B" has value of "1" etc. The method is essentialy an inverse of
"intToChar()" function */
int charToInt(char c);

/* Converts an integer value (i.e. "A" = 0, "B" = 1) to ASCII value.
The method is essentialy an inverse of "charToInt()" function */
char intToChar(int c);

#endif