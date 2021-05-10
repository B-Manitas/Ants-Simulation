#ifndef PRINCIPAL_HPP
#define PRINCIPAL_HPP

#include "grille.hpp"
#include "grilleFourmis.hpp"
#include "fourmi.hpp"
#include "place.hpp"
#include <string>
#include <vector>

std::string repeat_string(std::string str, int n);
void initialiserEmplacements(Grille &laGrille, GrilleFourmis &lesFourmis);
void dessineGrille(Grille &g, GrilleFourmis &lesFourmis, int n);
// void dessine2DVecteur(std::vector<std::vector<Fourmi>> &t);
void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title = "");

#endif