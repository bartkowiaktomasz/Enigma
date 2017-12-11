#include "helper.h"
#include "reflector.h"
#include "constants.h"
#include "errors.h"

#include <iostream>
#include <fstream>

using namespace std;

Reflector::Reflector(vector<int> configNumbers): configNumbers_(configNumbers) {}

int Reflector::createReflector(char* reflectorConfigFile, Reflector** reflector)
{
	int number;
	string line;
	vector<int> configNumbers;

	ifstream inputFile;
	inputFile.open(reflectorConfigFile);

	if(inputFile.fail())
	{
		cerr << "Error opening configuration file reflector.rf" << endl;
		return ERROR_OPENING_CONFIGURATION_FILE;
	}

	while((inputFile >> number) && !inputFile.eof())
	{
		if(number < 0 || number >= ALPHABET_SIZE)
		{
			cerr << "Invalid index in file reflector.rf" << endl;
			return INVALID_INDEX;
		}

		if(configNumbers.size() >= ALPHABET_SIZE)
		{
			cerr << "Incorrect (odd) number of parameters in reflector file reflector.rf"
				 << endl;
			return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
		}

		/* If the file attempts to connect a contact with itself, or with
		more than one other contact */
		if(hasDuplicateElement(configNumbers, number))
		{
			cerr << "Invalid mapping in reflector file reflector.rf" << endl;
			return INVALID_REFLECTOR_MAPPING;
		}

		configNumbers.push_back(number);
	}

	/* If while loop terminated before the EOF, it must have encountered
	a NON_NUMERIC_CHARACTER */
	if(!inputFile.eof())
	{
		cerr << "Non-numeric character in reflector file reflector.rf"
			 << endl;
		return NON_NUMERIC_CHARACTER;
	}

	if(configNumbers.size() < ALPHABET_SIZE)
	{
		if(configNumbers.size() % 2 == 0)
		{
			cerr << "Insufficient number of mappings in reflector file: reflector.rf"
				 << endl;
			return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
		}
		else
		{
			cerr << "Incorrect (odd) number of parameters in reflector file reflector.rf"
				 << endl;
			return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
		}
	}

	*reflector = new Reflector(configNumbers);
	inputFile.close();
	return NO_ERROR;
}

int Reflector::map(int letter)
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
	return NO_ERROR;
}