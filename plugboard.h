#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include <vector>

class Plugboard
{	
	private:
		std::vector<int> configNumbers_;

	public:
		Plugboard(std::vector<int> configNumbers);

		/* Maps given character to other character based on Plugboard
		configuration. Returns NO_ERROR (0) if successfull or given
		error code otherwise */
		int map(int letter);

		/* Takes as an argument a plugboard configuration file name.
		It opens that file, reads it, checks configuration numbers and saves them
		to the vector if valid. It returns NO_ERROR (0) if successfull or given
		error code otherwise */
		static int createPlugboard(char* plugboardConfigFile, Plugboard** plugboard);
};

#endif