#ifndef COORD_H
#define COORD_H

#include <iostream>
#include <vector>

const int TAILLEGRILLE = 5;

class Coord
{
private:
  int m_x;
  int m_y;

public:
  Coord(){};
  Coord(int x, int y);
  Coord getCoord() const;
  int getY() const;
  int getX() const;
  friend std::ostream &operator<<(std::ostream &out, const Coord &coord);
  bool operator==(const Coord &coord) const;
  bool operator!=(const Coord &coord) const;
};

class EnsCoord
{
private:
  std::vector<Coord> m_coords;

public:
  int position(const Coord &c) const;
  EnsCoord(){};
  EnsCoord(std::vector<Coord> coords) : m_coords{coords} {};
  friend std::ostream &operator<<(std::ostream &out, const EnsCoord &tableau);
  bool operator==(const EnsCoord &ens) const;
  bool contient(const Coord &c) const;
  void ajoute(const Coord c);
  void supprime(const Coord c);
  bool estVide() const;
  int taille() const;
  Coord ieme(const int &n) const;
};

EnsCoord voisines(Coord const &c);
Coord choixHasard(EnsCoord const &ens);

#endif
