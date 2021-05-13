#include "action.hpp"
#include "fourmi.hpp"
#include "place.hpp"
#include "doctest.h"

bool condition1(Fourmi f, Place p2) { return p2.contientFourmi(); };
bool condition2(Fourmi f, Place p2) { return f.chercheSucre() && p2.contientSucre(); };
bool condition3(Fourmi f, Place p2) { return not f.chercheSucre() && p2.contientNid(); };
bool condition4(Fourmi f, Place p1, Place p2) { return not f.chercheSucre() && p2.estVide() && estPlusProcheNid(p2, p1); };
bool condition5(Fourmi f, Place p1, Place p2) { return f.chercheSucre() && p1.estSurUnePiste() && p2.estVide() && not estPlusProcheNid(p2, p1) && p2.estSurUnePiste(); };
bool condition6(Fourmi f, Place p1, Place p2) { return f.chercheSucre() && p2.estSurUnePiste() && p2.estVide(); };
bool condition7(Fourmi f, Place p2) { return f.chercheSucre() && p2.estVide(); };
bool condtion_n(int n, Fourmi f, Place p1, Place p2)
{
  if (n == 1)
    return condition1(f, p1);

  else if (n == 2)
    return condition2(f, p2);

  else if (n == 3)
    return condition3(f, p2);

  else if (n == 4)
    return condition4(f, p1, p2);

  else if (n == 5)
    return condition5(f, p1, p2);

  else if (n == 6)
    return condition6(f, p1, p2);

  else if (n == 7)
    return condition7(f, p1);

  else
    return false;
}

void action2(Fourmi &f, Place &p1, Place &p2)
{
  f.porteSucre();
  p2.enleveSucre(1);
  p1.posePheroSucre();
}

void action3(Fourmi &f)
{
  f.poseSucre();
}

void action4(Fourmi &f, Place &p1, Place &p2)
{
  deplaceFourmi(f, p1, p2);
  p2.posePheroSucre();
}

void action567(Fourmi &f, Place &p1, Place &p2)
{
  deplaceFourmi(f, p1, p2);
}

void action_n(int n, Fourmi &f, Place &p1, Place &p2)
{
  if (n == 2)
    action2(f, p1, p2);

  else if (n == 3)
    action3(f);

  else if (n == 4)
    action4(f, p1, p2);

  else if (n == 5 or n == 6 or n == 7)
    action567(f, p1, p2);
}

TEST_SUITE_BEGIN("Test de la classe Action");
TEST_CASE("Test de la méthode action2")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(c);

  p2.poseSucre();
  action2(f, p1, p2);

  CHECK_FALSE(f.chercheSucre());
  CHECK(p1.contientPheroSucre());
}

TEST_CASE("Test de la méthode action3")
{
  Fourmi f = Fourmi(Coord(0, 0), 0);
  action3(f);
  CHECK(f.chercheSucre());
}

TEST_CASE("Test de la méthode action4")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));
  p1.poseSucre();

  action4(f, p1, p2);
  CHECK_FALSE(p1.contientFourmi());
  CHECK(p2.contientFourmi());
  CHECK(p2.contientPheroSucre());
  CHECK(p2.getNumeroFourmi() == f.getNum());
}

TEST_CASE("Test de la méthode action567")
{
  Coord c = Coord(0, 0);
  Fourmi f = Fourmi(c, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(1, 0));

  action567(f, p1, p2);
  CHECK_FALSE(p1.contientFourmi());
  CHECK(p2.contientFourmi());
  CHECK(p2.getNumeroFourmi() == f.getNum());
}
TEST_SUITE_END();