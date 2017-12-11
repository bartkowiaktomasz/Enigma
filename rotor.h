#ifndef ROTOR_H
#define ROTOR_H

#include <vector>

class Rotor
{
	private:
		std::vector<int> configNumbers_;
		std::vector<int> notchesPositions_;
		int rotationCounter_;

		/* Returns true if turnover notch aligns with "0" absolute position,
		false otherwise */
		bool isNotchAligned();

	public:
		Rotor(const std::vector<int> &configNumbers, const std::vector<int> &notchesPositions);

		/* Pointer to the previous rotor (the one to the left) */
		Rotor* previous;

		/* Rotates the Rotor to its initial position as stated
		in the .pos file. The rotations are made with disactivated
		turnover notches */
		void setStartingPosition(int numberOfRotations);

		/* Maps given character (on a way to Reflector) to
		other character based on Rotor configuration. Returns
		NO_ERROR (0) if successfull or given error code otherwise */
		int map(int letter);

		/* Maps given character (on a way back from Reflector) to
		other character based on Rotor configuration. Returns
		NO_ERROR (0) if successfull or given error code otherwise */
		int mapBack(int letter);

		/* Increments rotation counter and if a turnover notch is
		aligned with "0" position, rotates the neighouring rotor
		as well */
		void rotate(bool isNotchesActive);

		/* Takes as an argument a Rotor configuration file name.
		It opens that file, reads it, checks configuration numbers and saves them
		to the vector if valid. It returns NO_ERROR (0) if successfull or given
		error code otherwise */
		static int createRotor(char* rotorConfigFile, Rotor** rotor);
};

#endif