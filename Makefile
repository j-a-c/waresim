all:
	clear; clear;
	g++ -Wall -Wextra waresim.cpp */*.cpp */*/*.cpp */*/*/*.cpp -o waresim -pthread -std=c++11 -lGLU -lGLEW -lglfw
#-DGL_GLEXT_PROTOTYPES
