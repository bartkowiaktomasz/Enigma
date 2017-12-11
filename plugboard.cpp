#include "helper.h"
#include "plugboard.h"
#include "constants.h"
#include "errors.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Plugboard::Plugboard(vector<int> configNumbers): configNumbers_(configNumbers) {}

int Plugboard::createPlugboard(char* plugboardConfigFile, Plugboard** plugboard)
{
	int number;
	vector<int> configNumbers;

	ifstream inputFile;
	inputFile.open(plugboardConfigFile);

	if(inputFile.fail())
	{
		cerr << "Error opening configuration file plugboard.pb" << endl;
		inputFile.close();
		return ERROR_OPENING_CONFIGURATION_FILE;
	}

	while((inputFile >> number) && !inputFile.eof())
	{
		if(number < 0 || number >= ALPHABET_SIZE)
		{
			cerr << "Invalid index in plugboard file plugboard.pb" << endl;
			inputFile.close();
			return INVALID_INDEX;
		}

		if(configNumbers.size() >= ALPHABET_SIZE)
		{
			cerr << "Incorrect number of parameters in plugboard file plugboard.pb"
				 << endl;
			inputFile.close();
			return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
		}

		/* If the file attempts to connect a contact with itself, or with
		more than one other contact */
		if(hasDuplicateElement(configNumbers, number))
		{
			cerr << "Impossible plugboard configuration in file plugboard.pb"
				 << endl;
			inputFile.close();
			return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
		}

		configNumbers.push_back(number);
	}

	/* If while loop terminated before the EOF, it must have encountered
	a NON_NUMERIC_CHARACTER */
	if(!inputFile.eof())
	{
		cerr << "Non-numeric character in plugboard file plugboard.pb" << endl;
		inputFile.close();
		return NON_NUMERIC_CHARACTER;
	}

	if(configNumbers.size() % 2 == 1)
	{
		cerr << "Incorrect number of parameters in plugboard file plugboard.pb"
			 << endl;
		inputFile.close();
		return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
	}

	*plugboard = new Plugboard(configNumbers);
	inputFile.close();
	return NO_ERROR;
}

int Plugboard::map(int letter)
{
	for(unsigned int i = 0; i < configNumbers_.size(); i++)
	{
		if(configNumbers_[i] == letter)
		{
			if(i % 2 == 0)
				return configNumbers_[i+1];
			else
				return configNumbers_[i-1];
		}
	}
	return letter;
}