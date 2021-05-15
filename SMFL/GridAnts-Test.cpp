#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"
#include "GridAnts.hpp"

TEST_SUITE_BEGIN("GridAnts class tests.");
TEST_CASE("Test des méthodes de la classe GridAnts.")
{
  Ant f = Ant(Coord(0, 0), 1);
  Ant f2 = Ant(Coord(1, 0), 2);
  GridAnts g;
  g.setAnt(f);
  g.setAnt(f2);
  CHECK(g.getAnt(f.getCoord()).getIndex() == f.getIndex());
  CHECK(g.getSize() == 2);
}

TEST_CASE("GridAnts getSize.")
{
  Ant ant = Ant(Coord(0, 0), 1);
  Ant ant_2 = Ant(Coord(2, 2), 2);

  GridAnts grid;
  grid.setAnt(ant);
  grid.setAnt(ant_2);
  CHECK(grid.getSize() == 2);
}

TEST_CASE("GridAnts getAnt and setAnt.")
{
  Ant ant = Ant(Coord(0, 0), 1);
  Ant ant_2 = Ant(Coord(2, 2), 2);

  GridAnts grid;
  grid.setAnt(ant);
  grid.setAnt(ant_2);

  Ant ant_3 = grid.getAnt(Coord(0, 0));
  Ant ant_4 = grid.getAnt(Coord(2, 2));
  CHECK(ant_3 == ant);
  CHECK(ant_4 == ant_2);
}

TEST_CASE("GridAnts isContainingAnts.")
{
  Ant ant = Ant(Coord(0, 0), 1);
  Ant ant_2 = Ant(Coord(2, 2), 2);

  GridAnts grid;
  grid.setAnt(ant);
  grid.setAnt(ant_2);

  CHECK_FALSE(grid.isContainingAnts(4));
  CHECK(grid.isContainingAnts(1));
}

TEST_CASE("GridAnts isContainingAntsCoord.")
{
  Ant ant = Ant(Coord(0, 0), 1);
  Ant ant_2 = Ant(Coord(2, 2), 2);

  GridAnts grid;
  grid.setAnt(ant);
  grid.setAnt(ant_2);

  CHECK_FALSE(grid.isContainingAntsCoord(Coord(4, 4)));
  CHECK(grid.isContainingAntsCoord(Coord(0, 0)));
}

TEST_CASE("GridAnts remove.")
{
  Ant ant = Ant(Coord(0, 0), 1);
  Ant ant_2 = Ant(Coord(2, 2), 2);

  GridAnts grid;
  grid.setAnt(ant);
  grid.setAnt(ant_2);

  grid.remove(ant);

  CHECK(grid.isContainingAntsCoord(ant_2.getCoord()));
  CHECK_FALSE(grid.isContainingAntsCoord(ant.getCoord()));
}

TEST_SUITE_END();