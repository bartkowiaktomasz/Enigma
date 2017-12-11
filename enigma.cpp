#include "helper.h"
#include "enigma.h"
#include "constants.h"
#include "errors.h"

#include <iostream>
#include <fstream>

using namespace std;


Enigma::Enigma(Plugboard* plugboard, Reflector* reflector, const std::vector<Rotor*> &rotors):
	plugboard_(plugboard), reflector_(reflector), rotors_(rotors), numberOfRotors_(rotors.size()) {}

Enigma::~Enigma()
{
	delete plugboard_;
	delete reflector_;
	for(unsigned int i = 0; i < rotors_.size(); i++)
		delete rotors_[i];
}

void Enigma::encryptChar(char& c)
{
	bool isNotchesActive = true;
	int letter = charToInt(c);
	
	// Map by the Plugboard
	letter = plugboard_->map(letter);

	// Rotate the rightmost Rotor (if it exists) before closing the circuit
	if(numberOfRotors_ > 0)
		rotors_[numberOfRotors_ - 1]->rotate(isNotchesActive);

	// Map by the Rotors, start with the rightmost one (last in the vector)
	for(int i = numberOfRotors_ - 1; i >= 0; i--)
		letter = rotors_[i]->map(letter);	

	// Map by the Reflector
	letter = reflector_->map(letter);

	// Map back by the Rotors, start with the leftmost one (first in the vector)
	for(int i = 0; i < numberOfRotors_ ; i++)
		letter = rotors_[i]->mapBack(letter);
	
	// Map back the Plugboard
	letter = plugboard_->map(letter);

	c = intToChar(letter);
}

void Enigma::linkRotors(std::vector<Rotor*> &rotors)
{
	if(rotors.size() > 0)
	{
		rotors[0]->previous = NULL;
		for(unsigned int i = 1; i < rotors.size(); i++)
			rotors[i]->previous = rotors[i - 1];	
	}
}

int Enigma::setRotorsStartingPosition(std::vector<Rotor*> &rotors, char* rotorsPositionFile)
{
	int number;

	// Vector with initial positions of each rotor
	vector<int> positionNumbers;

	ifstream inputFile;
	inputFile.open(rotorsPositionFile);

	if(inputFile.fail())
	{		
		cerr << "Error opening rotor position file" << endl;
		inputFile.close();
		return ERROR_OPENING_CONFIGURATION_FILE;
	}

	while((inputFile >> number) && !inputFile.eof())
	{

		if(number < 0 || number >= ALPHABET_SIZE)
		{
			cerr << "Invalid index in rotor position file" << endl;
			inputFile.close();
			return INVALID_INDEX;
		}
		positionNumbers.push_back(number);	
	}

	/* If while loop terminated before the EOF, it must have encountered
	a NON_NUMERIC_CHARACTER */
	if(!inputFile.eof())
	{
		cerr << "Non-numeric character in rotor position file" << endl;
		inputFile.close();
		return NON_NUMERIC_CHARACTER;
	}

	if(positionNumbers.size() != rotors.size())
	{
		cerr << "No starting position" << endl;
		inputFile.close();
		return NO_ROTOR_STARTING_POSITION;
	}

	for(unsigned int i = 0; i < rotors.size(); i++)
		rotors[i]->setStartingPosition(positionNumbers[i]);
	
	inputFile.close();
	return NO_ERROR;	
}

int Enigma::encryptMessage(string& message)
{
    string word;
    while (!cin.eof())
    {
        cin >> word;
        for(unsigned int i = 0; i < word.length(); i++)
        {
        	if(!(isupper(word[i]) || word[i] == HORIZONTAL_TAB_CHAR ||
        	word[i] == NEW_LINE_CHAR || word[i] == CARRIAGE_RETURN_CHAR ||
        	word[i] == SPACE_CHAR))
        	{
        		cerr << word[i] << " is not a valid input character (input characters "
        			"must be upper case letters A-Z)!" << endl;
        		return INVALID_INPUT_CHARACTER;
        	}
        	encryptChar(word[i]);
        	cout << word[i];
        }
        message.append(word);
        cin >> ws;
    }
    return NO_ERROR;
}

int Enigma::createEnigmaComponents(int argc, char** argv, int numberOfRotors, Enigma** enigmaMachine)
{
	int exitCode;
	char* plugboardConfigFile = argv[1];
	char* reflectorConfigFile = argv[2];
	char* rotorsPositionFile;

	vector<char*> rotorsConfigFiles;
	for(int i = 3; i < argc - 1; i++)
		rotorsConfigFiles.push_back(argv[i]);

 	if(argc > 3)
 		rotorsPositionFile = argv[argc - 1];

 	// Create and configure a Plugboard
	Plugboard* plugboard;
	exitCode = Plugboard::createPlugboard(plugboardConfigFile, &plugboard);
	if(exitCode)
		return exitCode;

	// Create and configure a Reflector
	Reflector* reflector;
	exitCode = Reflector::createReflector(reflectorConfigFile, &reflector);
	if(exitCode)
	{
		delete plugboard;
		return exitCode;
	}

	// Create Rotors and check their configuration files
	vector<Rotor*> rotors(numberOfRotors);

	for(int i = 0; i < numberOfRotors; i++)
	{
		exitCode = Rotor::createRotor(rotorsConfigFiles[i], &rotors[i]);
		if(exitCode)
		{
			delete reflector;
			delete plugboard;
			return exitCode;
		}
	}

	// Link Rotors (if any) and set their initial positions
	if(numberOfRotors > 0)
	{
		linkRotors(rotors);
		exitCode = setRotorsStartingPosition(rotors, rotorsPositionFile);
		if(exitCode)
		{	
			for(int i = 0; i < numberOfRotors; i++)
				delete rotors[i];
			delete reflector;
			delete plugboard;
			return exitCode;
		}
	}

	*enigmaMachine = new Enigma(plugboard, reflector, rotors);
	return NO_ERROR;
}