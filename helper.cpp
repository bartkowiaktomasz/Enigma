#include "helper.h"

#include <iostream>
#include <algorithm>

using namespace std;

bool hasDuplicateElement(vector<int> numberVector, int number)
{
	if(find(numberVector.begin(), numberVector.end(), number) != numberVector.end())
	{
		return true;
	}
	else
		return false;
}

int charToInt(char c)
{
	return  c - LETTER_CAPITAL_A;
}

char intToChar(int c)
{
	return c + LETTER_CAPITAL_A;
}
