#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>

class Reflector
{
	private:
		std::vector<int> configNumbers_;

	public:
		Reflector(std::vector<int> configNumbers);

		/* Maps given character to other character based on Reflector
		configuration. Returns NO_ERROR (0) if successfull or given
		error code otherwise */
		int map(int letter);

		/* Takes as an argument a Reflector configuration file name.
		It opens that file, reads it, checks configuration numbers and saves them
		to the vector if valid. It returns NO_ERROR (0) if successfull or given
		error code otherwise */
		static int createReflector(char* reflectorConfigFile, Reflector** reflector);
};

#endif