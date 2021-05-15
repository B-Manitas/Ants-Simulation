#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"
#include "GridAnts.hpp"

TEST_SUITE_BEGIN("Test de la classe GridAnts.");
TEST_CASE("Test des méthodes de la classe GridAnts.")
{
  Ant f = Ant(Coord(0, 0), 1);
  Ant f2 = Ant(Coord(1, 0), 2);
  GridAnts g = GridAnts();
  g.setAnt(f);
  g.setAnt(f2);
  CHECK(g.getAnt(f.getCoord()).getIndex() == f.getIndex());
  CHECK(g.getSize() == 2);
}
TEST_SUITE_END();