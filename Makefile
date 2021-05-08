test.o: test.cpp coord.hpp fourmi.hpp place.hpp grille.hpp action.hpp
	g++ -Wall -std=c++11 -c test.cpp

test: test.o coord.o fourmi.o place.o grille.o action.o
	g++ -Wall -std=c++11 -o test test.o coord.o fourmi.o place.o grille.o action.o

run-test: test
	./test
	