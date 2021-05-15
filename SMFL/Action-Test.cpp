#include "Action.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"
#include "Place.hpp"
#include <iostream>

TEST_SUITE_BEGIN("Conditions tests.");
TEST_CASE("Condition 1")
{
  Place p2 = Place(Coord());
  p2.putAnt(Ant(Coord(0, 0), 0));
  CHECK(condition1(Ant(Coord(1, 0), 1), p2));
}

TEST_CASE("Condition 2")
{
  Place p2 = Place(Coord());
  p2.putSugar();
  CHECK(condition2(Ant(Coord(0, 0), 0), p2));
}

TEST_CASE("Condition 3")
{
  Place p2 = Place(Coord());
  p2.putAntNest();
  Ant ant = Ant(Coord(0, 0), 0);
  ant.carrySugar();
  CHECK(condition3(ant, p2));
}

TEST_CASE("Condition 4")
{
  Ant ant = Ant(Coord(0, 0), 0);
  ant.carrySugar();
  Place p1 = Place(Coord(1, 0));
  p1.putPheroAntNest(.1);

  Place p2 = Place(Coord());
  p2.putPheroAntNest(.2);
  CHECK(condition4(ant, p1, p2));
}

TEST_CASE("Condition 5")
{
  Ant ant = Ant(Coord(0, 0), 0);
  Place p1 = Place(Coord(1, 0));
  p1.putPheroSugar(10);
  p1.putPheroAntNest(.3);
  Place p2 = Place(Coord());
  p2.putPheroSugar(15);
  p2.putPheroAntNest(.2);

  CHECK(condition5(ant, p1, p2));
}

TEST_CASE("Condition 6")
{
  Ant ant = Ant(Coord(0, 0), 0);
  Place p2 = Place(Coord());
  p2.putPheroSugar(15);

  CHECK(condition6(ant, p2));
}

TEST_CASE("Condition 7")
{
  CHECK(condition7(Ant(Coord(0, 0), 0), Place(Coord(1, 0))));
}
TEST_SUITE_END();

TEST_SUITE_BEGIN("Action tests.");
TEST_CASE("Action 2")
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

TEST_CASE("Action 3")
{
  Coord coord(0, 0);
  Ant ant = Ant(coord, 0);
  Place p2 = Place(coord);
  p2.putAntNest();
  action3(ant, p2);
  CHECK(ant.lookForSugar());
}

TEST_CASE("Action 4")
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

TEST_CASE("actionMove")
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