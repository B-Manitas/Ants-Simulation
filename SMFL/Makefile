
# Variable SFML
LIBS = -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

# Déclare la variable RM selon l'OS
ifeq ($(OS),Windows_NT) 
RM = del
else
RM = rm -rf
endif

all: main clean
	./main

main.o: main.cpp coord.hpp fourmi.hpp place.hpp grille.hpp grilleFourmis.hpp action.hpp
	g++ -Wall -std=c++11 -Isrc/include -c main.cpp

main: main.o coord.o fourmi.o place.o grille.o grilleFourmis.o action.o
	g++ -Wall -std=c++11 -o main main.o coord.o fourmi.o place.o grille.o grilleFourmis.o action.o $(LIBS)

clean:
	$(RM) *.o