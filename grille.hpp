#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <vector>
#include "coord.hpp"
#include "place.hpp"

class Grille
{
private:
  std::vector<std::vector<Place>> grille;

public:
  Grille(){};
  Place chargePlace(Coord c);
  void rangePlace(Place p);
  void linearisePheroNid();
  void diminuePheroSucre();
};

#endif
