#ifndef MAIN_HPP
#define MAIN_HPP
#include "grille.hpp"
#include "grilleFourmis.hpp"

void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title);
std::vector<std::vector<int>> getValueGrille(Grille &laGrille, GrilleFourmis &lesFourmis);

#endif
