all:
	clear; clear;
	g++ -Wall -Wextra waresim/waresim.cpp */*/*.cpp */*/*/*.cpp */*/*/*/*.cpp -o waresim-run -pthread -std=c++11 -lGLU -lGLEW -lglfw -I/usr/include/freetype2 -lfreetype
#-DGL_GLEXT_PROTOTYPES
