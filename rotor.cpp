#include "helper.h"
#include "rotor.h"
#include "constants.h"
#include "errors.h"

#include <iostream>
#include <fstream>

using namespace std;

Rotor::Rotor(const std::vector<int> &configNumbers, const std::vector<int> &notchesPositions):
	configNumbers_(configNumbers), notchesPositions_(notchesPositions), rotationCounter_(0) {}

bool Rotor::isNotchAligned()
{
	for(unsigned int i = 0; i < notchesPositions_.size(); i++)
		if(notchesPositions_[i] == rotationCounter_)
			return true;
	return false;
}

int Rotor::createRotor(char* rotorConfigFile, Rotor** rotor)
{
	int number;
	vector<int> configNumbers;
	vector<int> notchesPositions;

	ifstream inputFile;
	inputFile.open(rotorConfigFile);

	// If opening input file fails
	if(inputFile.fail())
	{
		cerr << "Error opening rotor configuration file" << endl;
		inputFile.close();
		return ERROR_OPENING_CONFIGURATION_FILE;
	}

	while((inputFile >> number) && !inputFile.eof())
	{

		if(number < 0 || number >= ALPHABET_SIZE)
		{
			cerr << "Invalid index in rotor file" << endl;
			inputFile.close();
			return INVALID_INDEX;
		}

		// If the file attempts to connect a contact with itself, or with more than one other contact
		if(configNumbers.size() < ALPHABET_SIZE && hasDuplicateElement(configNumbers, number))
		{
			cerr << "Invalid rotor mapping" << endl;
			inputFile.close();
			return INVALID_ROTOR_MAPPING;
		}

		/* Add number to the configuration vector if it has less than 26 characters.
		Otherwise the number specifies a turnover notch position, so add it to the 
		notchesPositions vector */
		if(configNumbers.size() < ALPHABET_SIZE)
			configNumbers.push_back(number);
		else
			notchesPositions.push_back(number);
	}

	// If while loop terminated before the EOF, it must have encountered a NON_NUMERIC_CHARACTER
	if(!inputFile.eof())
	{
		cerr << "Non-numeric character" << endl;
		inputFile.close();
		return NON_NUMERIC_CHARACTER;
	}

	if(configNumbers.size() != ALPHABET_SIZE)
	{
		cerr << "Invalid rotor mapping" << endl;
		inputFile.close();
		return INVALID_ROTOR_MAPPING;
	}

	*rotor = new Rotor(configNumbers, notchesPositions);
	inputFile.close();
	return NO_ERROR;
}

void Rotor::setStartingPosition(int numberOfRotations)
{
	bool isNotchesActive = false;
	for(int i = 0; i < numberOfRotations; i++)
		rotate(isNotchesActive);
}

int Rotor::map(int letter)
{
	return (configNumbers_[(letter + rotationCounter_) % ALPHABET_SIZE]
		- rotationCounter_ + ALPHABET_SIZE) % ALPHABET_SIZE;
}

int Rotor::mapBack(int letter)
{
	for(unsigned int i = 0; i < configNumbers_.size(); i++)
	{
		if(configNumbers_[i] == (letter + rotationCounter_) % ALPHABET_SIZE)
			return (i - rotationCounter_ + ALPHABET_SIZE) % ALPHABET_SIZE;
	}
	return NO_ERROR;
}

void Rotor::rotate(bool isNotchesActive)
{
	rotationCounter_ = (rotationCounter_ + 1) % ALPHABET_SIZE;
 	 
 	// If notch hits 0 position, rotate previous rotor (if exists)
	if(isNotchesActive && isNotchAligned() && previous != NULL)
		previous->rotate(isNotchesActive);
}