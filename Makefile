# Makefile for Sudoku

CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 -g

main.exe: main.cpp Board.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

Board_tests.exe: Board_tests.cpp Board.cpp main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt
