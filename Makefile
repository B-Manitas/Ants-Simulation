test.o: test.cpp coord.hpp place.hpp grille.hpp
	g++ -Wall -std=c++11 -c test.cpp

test: test.o coord.o place.o grille.o
	g++ -Wall -std=c++11 -o test test.o coord.o place.o grille.o

run-test: test
	./test
	