
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


main.o: Main.hpp Coord.hpp Ant.hpp Place.hpp Action.hpp Grid.hpp GridAnts.hpp
	g++ -Wall -std=c++11 -Isrc/include -c main.cpp

main: main.o coord.o fourmi.o place.o action.o grille.o grilleFourmis.o
	g++ -Wall -std=c++11 -o main main.o coord.o fourmi.o place.o action.o grille.o grilleFourmis.o $(LIBS)

clean:
	$(RM) *.o