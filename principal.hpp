#ifndef PRINCIPAL_HPP
#define PRINCIPAL_HPP

#include "grille.hpp"
#include "grilleFourmis.hpp"
#include "fourmi.hpp"
#include "place.hpp"
#include <string>
#include <vector>

std::string repeat_string(std::string str, int n);
GrilleFourmis initialiserEmplacements(Grille &laGrille);
void dessineGrille(Grille &g, GrilleFourmis &lesFourmis, int n);
void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title = "");

#endif