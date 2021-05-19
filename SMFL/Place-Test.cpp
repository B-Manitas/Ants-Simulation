#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"
#include "Grid.hpp"
#include "Place.hpp"

#include <sstream>

TEST_SUITE_BEGIN("Place class tests.");

TEST_CASE("Place isEmpty")
{
  Place p;
  CHECK(p.isEmpty());
  p.putSugar(1);
  CHECK_FALSE(p.isEmpty());
}

TEST_CASE("Place putSugar")
{
  Place p;
  p.putSugar(1);
  CHECK(p.isContainingSugar());
  CHECK(p.getPheroSugar() == 255);

  Place p2 = Place(Coord(0, 0));
  p2.putAnt(Ant(Coord(0, 0), 0));
  CHECK_FALSE(p2.putSugar());
}

TEST_CASE("Place putAntNest an removeAntNest")
{
  Place p;
  p.putAntNest();
  CHECK(p.isContainingAntNest());
  CHECK(p.getPheroAntNest() == 1);
  CHECK_FALSE(p.putAnt(Ant()));
}

TEST_CASE("Place putAnt")
{
  // Testes de poser plusieurs Ants sur une même place.
  Place p;
  p.putAnt(Ant(Coord(), 0));
  CHECK(p.isContainingAnt());
  CHECK_FALSE(p.putAnt(Ant(Coord(), 1)));

  // Testes de poser un nid et une Ants sur une même place.
  Place p1;
  p1.putAntNest();
  CHECK_FALSE(p1.putAnt(Ant(Coord(), 0)));

  // Testes de poser un sucre et une Ants sur une même case.
  Place p2;
  p2.putSugar();
  CHECK_FALSE(p2.putAnt(Ant(Coord(), 0)));

  // Testes de poser un sucre et une Ants qui porte un sucre.
  Place p3 = Place(Coord());
  p3.putSugar();
  Ant f = Ant(Coord(), 0);
  f.carrySugar();
  CHECK(p3.putAnt(f));
  CHECK(p3.isContainingAnt());
}

TEST_CASE("Place putPheroAntNest")
{
  Place p;
  p.putPheroAntNest(.5);
  CHECK(p.getPheroAntNest() == .5);
}

TEST_CASE("Place putPheroSugar")
{
  Place p;
  p.putPheroSugar(100);
  CHECK(p.getPheroSugar() == 100);
}

TEST_CASE("Place removeSugar")
{
  Place p;
  p.putSugar(1);
  p.removeSugar();
  CHECK_FALSE(p.isContainingSugar());
  CHECK(p.getPheroSugar() == 255);
}

TEST_CASE("Place removeAnt")
{
  Place p;
  p.putAnt(Ant(Coord(), 0));
  p.removeAnt();
  CHECK_FALSE(p.isContainingAnt());
}

TEST_CASE("Place removePheroSugar")
{
  Place p;
  p.putPheroSugar(100);
  p.removePheroSugar();
  CHECK_FALSE(p.isContainingPheroSugar());
}

TEST_CASE("Place decreasePheroSugar")
{
  Place p;
  p.putPheroSugar(255);
  p.decreasePheroSugar(1);
  CHECK(p.getPheroSugar() == 254);
}

TEST_CASE("Place operator == et !=.")
{
  Place p = Place(Coord(0, 0));
  Place p2 = Place(Coord(1, 0));
  Place p3 = Place(Coord(1, 0));
  p3.putSugar();

  CHECK(p == p);
  CHECK(p != p2);
  CHECK(p2 != p3);
}

TEST_CASE("Place operator <<.")
{
  std::ostringstream stream;
  stream << Place(Coord(2, 3));

  std::ostringstream streamCheck;
  streamCheck << "{ Coord: (2, 3), "
              << "Id Ant: -1, "
              << "Phero Ant: 0, "
              << "Phero Sugar: 0, "
              << "Ant: No, "
              << "Sugar: No, "
              << "Water: No }";

  CHECK(stream.str() == streamCheck.str());
}
TEST_SUITE_END();

TEST_SUITE_BEGIN("Non-member function tests.");
TEST_CASE("Non-member function isCloserToNest.")
{
  Place p1, p2;
  p1.putPheroAntNest(.5);
  p2.putPheroAntNest(.3);

  CHECK(isCloserToNest(p1, p2));
}

TEST_CASE("Non-member function moveAnts.")
{
  Coord c = Coord(0, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(0, 1));
  Ant f = Ant(c, 1);

  moveAnt(f, p1, p2);
  CHECK_FALSE(p1.isContainingAnt());
  CHECK(p2.isContainingAnt());
  CHECK(f.getCoord() == p2.getCoord());
}
TEST_SUITE_END();