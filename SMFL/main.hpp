#ifndef MAIN_HPP
#define MAIN_HPP
#include "grille.hpp"
#include "grilleFourmis.hpp"

void consistencyTest(Grille &grid, GrilleFourmis &ants, std::string title);
std::vector<std::vector<int>> getGridState(Grille &grid, GrilleFourmis &ants);

#endif
