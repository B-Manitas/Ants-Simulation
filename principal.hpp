#ifndef PRINCIPAL_HPP
#define PRINCIPAL_HPP

#include "grille.hpp"
#include "grilleFourmis.hpp"
#include <string>

std::string repeat_string(std::string str, int n);
Grille initialiserEmplacements();
void dessineGrille(Grille g, GrilleFourmis lesFourmis, int n);

#endif