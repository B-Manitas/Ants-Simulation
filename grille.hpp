#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <vector>
#include <iostream>
#include "coord.hpp"
#include "place.hpp"
#include "fourmi.hpp"

struct Grille
{
  std::vector<std::vector<Place>> m_grille;

  Grille() : m_grille(std::vector<std::vector<Place>>()){};
  Grille(int taille);
  friend std::ostream &operator<<(std::ostream &out, const Grille &g);

  int taille() { return m_grille.size(); };
  Place chargePlace(Coord c);
  void rangePlace(Place p);
  void linearisePheroNid();
  void diminuePheroSucre();
  Place randPlace();
};

void mettreAJourUneFourmi(Fourmi fourmi, Grille laGrille);
void mettreAJourEnsFourmis(Grille laGrille, std::vector<Fourmi> lesFourmis);

#endif
