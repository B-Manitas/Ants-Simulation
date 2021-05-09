#ifndef GRILLE_FOURMIS_HPP
#define GRILLE_FOURMIS_HPP

#include <vector>
#include "fourmi.hpp"

struct GrilleFourmis
{
  std::vector<std::vector<Fourmi>> m_grilleF;

  GrilleFourmis() : m_grilleF(std::vector<std::vector<Fourmi>>()){};
  GrilleFourmis(int taille);

  int taille() { return m_grilleF.size(); };
  Fourmi chargeFourmi(Coord c);
  void rangeFourmi(Fourmi f);
  Fourmi chercheFourmi(int idFourmi);
};

#endif