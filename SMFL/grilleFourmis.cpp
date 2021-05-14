#include "grilleFourmis.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"

int GrilleFourmis::position(int const id)
{
  for (int i = 0; i < getSize(); i++)
    if (m_grid[i].getIndex() == id)
      return i;

  return -1;
}

bool GrilleFourmis::isContainingAnts(int const id) const
{
  for (auto &ant : m_grid)
    if (ant.getIndex() == id)
      return true;

  return false;
}

bool GrilleFourmis::isContainingAntsCoord(Coord const &coord) const
{
  for (auto &ant : m_grid)
    if (ant.getCoord() == coord)
      return true;

  return false;
}

Fourmi GrilleFourmis::getAnt(Coord const &coord) const
{
  for (auto &ant : m_grid)
    if (ant.getCoord() == coord)
      return ant;

  throw std::runtime_error("Aucune fourmi n'a été trouvé.");
}

void GrilleFourmis::setAnt(Fourmi const &ant)
{
  if (not isContainingAnts(ant.getIndex()))
    m_grid.push_back(ant);

  else
    m_grid[position(ant.getIndex())] = ant;
}

std::ostream &operator<<(std::ostream &out, GrilleFourmis const &ants)
{
  out << "{";
  for (int i = 0; i < ants.getSize(); i++)
  {
    out << ants.m_grid[i];
    if (i != ants.getSize() - 1)
      out << ", ";
  }
  out << "}";

  return out;
}

TEST_SUITE_BEGIN("Test de la classe GrilleFourmis.");
TEST_CASE("Test des méthodes de la classe GrilleFourmis.")
{
  Fourmi f = Fourmi(Coord(0, 0), 1);
  Fourmi f2 = Fourmi(Coord(1, 0), 2);
  GrilleFourmis g = GrilleFourmis();
  g.setAnt(f);
  g.setAnt(f2);
  CHECK(g.getAnt(f.getCoord()).getIndex() == f.getIndex());
  CHECK(g.getSize() == 2);
}
TEST_SUITE_END();
