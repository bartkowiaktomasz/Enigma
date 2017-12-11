/*
Enigma Machine
Usage: ./enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)
*/

#include "enigma.h"
#include "constants.h"
#include "errors.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv)
{
	int exitCode = NO_ERROR;
	int numberOfRotors;
	string message;

	if(argc < 3)
	{
		cerr << "usage: enigma plugboard-file reflector-file "
				"(<rotor-file>* rotor-positions)?" << endl;
		return INSUFFICIENT_NUMBER_OF_PARAMETERS;
	}

	/* If 3 arguments, no rotors are given (numberOfRotors = 0).
 	If 4 or more arguments given, set numberOfRotors to  argc - 4 */
	argc == 3 ? numberOfRotors = 0 : numberOfRotors = argc - 4;

	Enigma* enigmaMachine;
 	exitCode = Enigma::createEnigmaComponents(argc, argv, numberOfRotors, &enigmaMachine);
 	if(exitCode)
 		return exitCode;

	exitCode = enigmaMachine->encryptMessage(message);
	if(exitCode)
	{
		delete enigmaMachine;
 		return exitCode;
	}

	delete enigmaMachine;
	return NO_ERROR;	
}