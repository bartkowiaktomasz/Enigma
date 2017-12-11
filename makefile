OBJ = main.o enigma.o plugboard.o rotor.o reflector.o helper.o
EXE = enigma
CXX = g++
CXXFLAGS = -Wall -g

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean :
	rm -f $(OBJ) $(EXE)