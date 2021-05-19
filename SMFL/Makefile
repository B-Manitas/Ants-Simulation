
# Variable SFML
SRC_FILES= Action.hpp Ant.hpp Coord.hpp Grid.hpp GridAnts.hpp Place.hpp
OBJECT_FILES= Action.o Ant.o Coord.o Grid.o GridAnts.o Place.o

SRC_TEST_FILES= Ant-Test.cpp Action-Test.cpp Coord-Test.cpp Grid-Test.cpp GridAnts-Test.cpp Place-Test.cpp Test.cpp
OBJECT_TEST_FILES= Action-Test.o Ant-Test.o Coord-Test.o Grid-Test.o GridAnts-Test.o Place-Test.o

LIBS= -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system
CXX= g++ -Wall -std=c++11

# Déclare la variable RM selon l'OS
ifeq ($(OS),Windows_NT) 
RM = del
else
RM = rm -rf
endif

all: Main Test clean
	./Test
	./Main

simulation: Main
	./Main

test: Test clean
	./Test

Main.o: Main.cpp $(SRC_FILES)
	$(CXX) -Isrc/include -c Main.cpp

Main: Main.o $(OBJECT_FILES)
	$(CXX) -o Main Main.o $(OBJECT_FILES) $(LIBS)

Test.o: $(SRC_TEST_FILES) $(SRC_FILES)
	$(CXX) -c Test.cpp

Test: Test.o $(OBJECT_TEST_FILES) $(OBJECT_FILES)
	$(CXX) -o Test Test.o $(OBJECT_TEST_FILES) $(OBJECT_FILES)

clean:
	$(RM) *.o

clean-all:
	$(RM) *.exe
	$(RM) *.o