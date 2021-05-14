#include "Action.hpp"
#include "Ant.hpp"
#include "Place.hpp"
#include "doctest.h"

bool condition1(Fourmi ant, Place p2) { return p2.isContainingAnt(); };
bool condition2(Fourmi ant, Place p2) { return ant.lookForSugar() && p2.isContainingSugar(); };
bool condition3(Fourmi ant, Place p2) { return not ant.lookForSugar() && p2.isContainingAntNest(); };
bool condition4(Fourmi ant, Place p1, Place p2) { return not ant.lookForSugar() && p2.isEmpty() && isCloserToNest(p2, p1); };
bool condition5(Fourmi ant, Place p1, Place p2) { return ant.lookForSugar() && p1.isOnSugarTrail() && p2.isEmpty() && not isCloserToNest(p2, p1) && p2.isOnSugarTrail(); };
bool condition6(Fourmi ant, Place p1, Place p2) { return ant.lookForSugar() && p2.isOnSugarTrail() && p2.isEmpty(); };
bool condition7(Fourmi ant, Place p2) { return ant.lookForSugar() && p2.isEmpty(); };
bool condtionNth(int n, Fourmi ant, Place p1, Place p2)
{
  switch (n)
  {
  case 1:
    return false;
  case 2:
    return condition2(ant, p2);
  case 3:
    return condition3(ant, p2);
  case 4:
    return condition4(ant, p1, p2);
  case 5:
    return condition5(ant, p1, p2);
  case 6:
    return condition6(ant, p1, p2);
  case 7:
    return condition7(ant, p2);
  default:
    return false;
  }
}

void action2(Fourmi &ant, Place &p1, Place &p2)
{
  ant.carrySugar();
  p2.removeSugar(1);
  p1.putPheroSugar();
}

void action3(Fourmi &ant)
{
  ant.putSugar();
}

void action4(Fourmi &ant, Place &p1, Place &p2)
{
  moveAnt(ant, p1, p2);
  p2.putPheroSugar();
}

void actionMove(Fourmi &ant, Place &p1, Place &p2)
{
  moveAnt(ant, p1, p2);
}

void actionNth(int n, Fourmi &ant, Place &p1, Place &p2)
{
  switch (n)
  {
  case 1:
    break;
  case 2:
    action2(ant, p1, p2);
    break;

  case 3:
    action3(ant);
    break;

  case 4:
    action4(ant, p1, p2);
    break;

  case 5:
  case 6:
  case 7:
    actionMove(ant, p1, p2);
    break;

  default:
    break;
  }
}

TEST_SUITE_BEGIN("Test de la classe Action");
TEST_CASE("Test de la méthode action2")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(c);

  p2.putSugar();
  action2(f, p1, p2);

  CHECK_FALSE(f.lookForSugar());
  CHECK(p1.isContainingPheroSugar());
}

TEST_CASE("Test de la méthode action3")
{
  Fourmi f = Fourmi(Coord(0, 0), 0);
  action3(f);
  CHECK(f.lookForSugar());
}

TEST_CASE("Test de la méthode action4")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
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
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));

  actionMove(f, p1, p2);
  CHECK_FALSE(p1.isContainingAnt());
  CHECK(p2.isContainingAnt());
  CHECK(p2.getIdAnt() == f.getIndex());
}
TEST_SUITE_END();