#include "action.hpp"
#include "fourmi.hpp"
#include "place.hpp"
#include "doctest.h"

bool Action::condtion_n(int n, Fourmi f, Place p1, Place p2) const
{
  switch (n)
  {
  case 1:
    return condition1(f, p1);
  case 2:
    return condition2(f, p1);
  case 3:
    return condition3(f, p1);
  case 4:
    return condition4(f, p1, p2);
  case 5:
    return condition5(f, p1, p2);
  case 6:
    return condition6(f, p1, p2);
  case 7:
    return condition7(f, p1);

  default:
    return false;
  }
}

void Action::action2(Fourmi &f, Place &p1)
{
  f.porteSucre();
  p1.posePheroSucre();
}

void Action::action3(Fourmi &f)
{
  f.poseSucre();
}

void Action::action4(Fourmi &f, Place &p1, Place &p2)
{
  deplaceFourmi(f, p1, p2);
  p2.posePheroSucre();
}

void Action::action7(Fourmi &f, Place &p1, Place &p2)
{
  deplaceFourmi(f, p1, p2);
}

void Action::action_n(int n, Fourmi &f, Place &p1, Place &p2)
{
  switch (n)
  {
  case 2:
    action2(f, p1);
    break;

  case 3:
    action3(f);
    break;

  case 4:
    action4(f, p1, p2);
    break;

  case 7:
    action7(f, p1, p2);
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

  p1.poseSucre();
  Action().action2(f, p1);

  CHECK_FALSE(f.chercheSucre());
  CHECK(p1.contientPheroSucre());
}

TEST_CASE("Test de la méthode action3")
{
  Fourmi f = Fourmi(Coord(0, 0), 0);
  Action().action3(f);
  CHECK(f.chercheSucre());
}

TEST_CASE("Test de la méthode action4")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));
  p1.poseSucre();

  Action().action4(f, p1, p2);
  CHECK_FALSE(p1.contientFourmi());
  CHECK(p2.contientFourmi());
  CHECK(p2.contientPheroSucre());
  CHECK(p2.getNumeroFourmi() == f.getNum());
}

TEST_CASE("Test de la méthode action7")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));

  Action().action7(f, p1, p2);
  CHECK_FALSE(p1.contientFourmi());
  CHECK(p2.contientFourmi());
  CHECK(p2.getNumeroFourmi() == f.getNum());
}
TEST_SUITE_END();