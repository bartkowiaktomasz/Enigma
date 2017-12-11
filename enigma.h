#ifndef ENIGMA_H
#define ENIGMA_H

#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"

#include <vector>
#include <string>

class Enigma
{
	private:
		Plugboard* plugboard_;
		Reflector* reflector_;
		std::vector<Rotor*> rotors_;

		int numberOfRotors_;

		/* Takes as an argument a refrence to a character that is
		to be encrypted. The character is then passed through the components
		of the enigma machine, being altered at each stage of encryption */
		void encryptChar(char& c);

		/* Links rotors, meaning that for each rotor it
		creates a pointer "previous" pointing to the neighbouring rotor
		to the left. The leftmost (first) rotor is assigned a NULL pointer */
		static void linkRotors(std::vector<Rotor*> &rotors);

		/* Takes the path name to the rotors position file (.pos)
		as an argument. It opens that file, checks for errors and invokes
		Rotor::setStartingPosition() method to set rotors' initial positions.
		Returns NO_ERROR (0) if successfull or given error code
		otherwise */
		static int setRotorsStartingPosition(std::vector<Rotor*> &rotors, char* rotorsPositionFile);

	public:
		/* Constructor initialises the Enigma components' pointers to NULL
		and sets the number of rotors */
		Enigma(Plugboard* plugboard, Reflector* reflector, const std::vector<Rotor*> &rotors);

		~Enigma();

		/* Given input message, checks each character of the input,
		and if it is valid, invokes the encryption method on that character. 
		Returns NO_ERROR (0) if successfull or given error code
		otherwise. */
		int encryptMessage(std::string& message);

		/* Creates new instances of Enigma components' classes
		(Plugboard, Reflector and Rotor) and invokes configuration 
		methods on each element. It also sets Rotors to their initial
		positions and linkes together. Returns NO_ERROR (0) if
		successfull or given error code otherwise  */ 
		static int createEnigmaComponents(int argc, char** argv, int numberOfRotors, Enigma** enigmaMachine);
};


#endif