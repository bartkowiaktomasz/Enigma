# Enigma
This repository contains an implementation of Enigma machine used by German forces in the Second World War. The project was done as a part of C++ course at Imperial and was graded A*.
The Enigma machine can encrypt and decrypt messages that are written in a fixed sized alphabet (26 upper-case Latin letters A-Z). The message to be encoded should be stored in input.txt file.

Usage:
`./enigma plugboard-file reflector-file (<rotor-file>* rotor-positions) < input.txt`

Example use:
`./enigma plugboards/I.pb reflectors/I.rf rotors/I.rot rotors/II.rot rotors/III.rot rotors/I.pos < input.txt`
