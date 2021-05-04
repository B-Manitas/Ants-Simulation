#ifndef COORD_H
#define COORD_H

#include <iostream>
#include <vector>

const int TAILLEGRILLE = 5;

class Coord
{
private:
  int lig;
  int col;

public:
  Coord(int lig, int col);
  Coord getCoord() const;
  int getLig() const;
  int getCol() const;
  friend std::ostream &operator<<(std::ostream &out, const Coord &coord);
  bool operator==(const Coord &coord) const;
  bool operator!=(const Coord &coord) const;
};

class EnsCoord
{
private:
  std::vector<Coord> coords;
  int position(const Coord &c) const;

public:
  EnsCoord(){};
  EnsCoord(std::vector<Coord> coords) : coords{coords} {};
  friend std::ostream &operator<<(std::ostream &out, const EnsCoord &tableau);
  bool operator==(const EnsCoord &ens) const;
  bool contient(const Coord &c) const;
  void ajoute(const Coord c);
  void supprime(const Coord c);
  bool estVide() const;
  int taille() const;
  Coord ieme(const int &n) const;
};

EnsCoord voisines(Coord &c);
Coord choixHasard(EnsCoord const &ens);

#endif
