all:	Icarus
Icarus: main.o Parse.o
	g++ main.o Parse.o -o Icarus
main.o: main.cpp Parse/Parse.cpp
	g++ -c main.cpp
Parse.o: Parse/Parse.cpp
	g++ -c Parse/Parse.cpp
