#ifndef GRILLE_FOURMIS_HPP
#define GRILLE_FOURMIS_HPP

#include <vector>
#include "fourmi.hpp"

struct GrilleFourmis
{
  std::vector<Fourmi> m_grilleF;

  GrilleFourmis() : m_grilleF(std::vector<Fourmi>()){};
  // GrilleFourmis(int taille);

  int taille() { return m_grilleF.size(); };
  bool contientFourmis(int idFourmi);
  bool contientFourmisCoord(Coord c);
  // void supprime(Coord c);
  int position(int const &idFourmi);
  Fourmi chargeFourmi(Coord c);
  void rangeFourmi(Fourmi &f);
  Fourmi chercheFourmi(int idFourmi);
  friend std::ostream &operator<<(std::ostream &out, GrilleFourmis &grille);
};

#endif