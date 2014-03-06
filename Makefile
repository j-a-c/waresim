all:
	clear; clear;
	g++ -Wall -Wextra waresim.cpp sim/*.cpp -o waresim -pthread -std=c++11
