# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lsndfile

# Target program
audio_program: main.o Audioer.o
	$(CXX) $(CXXFLAGS) -o audio_program main.o Audioer.o $(LDFLAGS)

# Rule to build main.o
main.o: main.cpp Audioer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Rule to build Audioer.o
Audioer.o: Audioer.cpp Audioer.h
	$(CXX) $(CXXFLAGS) -c Audioer.cpp

# Clean up generated files
clean:
	rm -f *.o audio_program
