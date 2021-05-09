ifeq ($(OS),Windows_NT) 
RM = del
else
RM = rm -rf
endif

test.o: test.cpp coord.hpp fourmi.hpp place.hpp grille.hpp action.hpp
	g++ -Wall -std=c++11 -c test.cpp

test: test.o coord.o fourmi.o place.o grille.o action.o
	g++ -Wall -std=c++11 -o test test.o coord.o fourmi.o place.o grille.o action.o

run-test: test
	-$(RM) *.o
	./test

principal.o: principal.hpp coord.hpp fourmi.hpp place.hpp grille.hpp grilleFourmis.hpp action.hpp
	g++ -Wall -std=c++11 -c principal.cpp

principal: principal.o coord.o fourmi.o place.o grille.o grilleFourmis.o action.o
	g++ -Wall -std=c++11 -o principal principal.o coord.o fourmi.o place.o grille.o grilleFourmis.o action.o

run-principal: principal
	-$(RM) *.o
	./principal
