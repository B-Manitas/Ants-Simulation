test.o: test.cpp coord.hpp
	g++ -Wall -std=c++11 -c test.cpp


test: test.o coord.o
	g++ -Wall -std=c++11 -o test test.o coord.o

run-test: test
	./test
	