all:
	clear; clear;
	g++ -Wall -Wextra waresim/waresim.cpp */*/*.cpp */*/*/*.cpp */*/*/*/*.cpp -o waresim.out -pthread -std=c++11 -lGLU -lGLEW -lglfw
#-DGL_GLEXT_PROTOTYPES
