#include "Action.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"
#include "Place.hpp"

TEST_SUITE_BEGIN("Test de la classe Action");
TEST_CASE("Test de la méthode action2")
{
  Coord c = Coord(0, 0);
  Ant f = Ant(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(c);

  p2.putSugar();
  action2(f, p1, p2);

  CHECK_FALSE(f.lookForSugar());
  CHECK(p1.isContainingPheroSugar());
}

TEST_CASE("Test de la méthode action3")
{
  Coord coord(0, 0);
  Ant ant = Ant(coord, 0);
  Place p2 = Place(coord);
  p2.putAntNest();
  action3(ant, p2);
  CHECK(ant.lookForSugar());
}

TEST_CASE("Test de la méthode action4")
{
  Coord c = Coord(0, 0);
  Ant f = Ant(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));
  p1.putSugar();

  action4(f, p1, p2);
  CHECK_FALSE(p1.isContainingAnt());
  CHECK(p2.isContainingAnt());
  CHECK(p2.isContainingPheroSugar());
  CHECK(p2.getIdAnt() == f.getIndex());
}

TEST_CASE("Test de la méthode actionMove")
{
  Coord c = Coord(0, 0);
  Ant f = Ant(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));

  actionMove(f, p1, p2);
  CHECK_FALSE(p1.isContainingAnt());
  CHECK(p2.isContainingAnt());
  CHECK(p2.getIdAnt() == f.getIndex());
}
TEST_SUITE_END();