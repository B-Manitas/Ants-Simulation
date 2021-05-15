#include "Coord.hpp"
#include "Doctest.h"
#include "Grid.hpp"
#include "Place.hpp"

TEST_SUITE_BEGIN("Test de la classe Grid.");
TEST_CASE("Test de la fonction getPlace.")
{
  CHECK(Grid(1).getPlace(Coord(0, 0)) == Place(Coord(0, 0)));
  CHECK(Grid(5).getPlace(Coord(4, 4)) == Place(Coord(4, 4)));
  CHECK_THROWS_AS(Grid(5).getPlace(Coord(5, 4)), std::invalid_argument);
  CHECK_THROWS_AS(Grid(5).getPlace(Coord(4, 5)), std::invalid_argument);
}

TEST_CASE("Test de la fonction getPlace.")
{
  Grid g = Grid(2);
  Coord c = Coord(1, 0);
  Place p = g.getPlace(c);
  p.putSugar();
  g.setPlace(p);

  CHECK(g.getPlace(c) == p);
}
TEST_SUITE_END();