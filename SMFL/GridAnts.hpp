#ifndef GRILLE_FOURMIS_HPP
#define GRILLE_FOURMIS_HPP

#include <vector>
#include "Ant.hpp"

struct GrilleFourmis
{
  std::vector<Fourmi> m_grid;

  GrilleFourmis() : m_grid(std::vector<Fourmi>()){};
  int position(int const id);

  int getSize() const { return m_grid.size(); };
  Fourmi getAnt(Coord const &coord) const;
  void setAnt(Fourmi const &ant);

  bool isContainingAnts(int const id) const;
  bool isContainingAntsCoord(Coord const &coord) const;

  friend std::ostream &operator<<(std::ostream &out, GrilleFourmis const &ants);
};

#endif