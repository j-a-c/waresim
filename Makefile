all:
	clear; clear;
	g++ -Wall -Wextra waresim.cpp simulation.cpp order_generator.cpp -o waresim -pthread -std=c++11
