#include "Place.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "doctest.h"
#include <sstream>

bool Place::putSugar(int quantity)
{
  if (not isContainingAnt())
  {
    m_pieceSugar += quantity;
    putPheroSugar(255);
    return true;
  }

  return false;
};

bool Place::putAnt(Fourmi ant)
{
  if (isContainingAnt() or isContainingAntNest() or (isContainingSugar() && ant.lookForSugar()))
    return false;

  m_idAnt = ant.getIndex();
  return true;
};

bool Place::putAntNest()
{
  if (not isContainingAnt())
  {
    m_pieceAntNest++;
    putPheroAntNest(1);
    return true;
  }

  return false;
};

void Place::putPheroSugar(double quantity)
{
  if (quantity > 0)
    m_pheroSugar += (quantity > 255) ? 255 : quantity;

  m_pheroSugar = std::min(m_pheroSugar, 255);
};

void Place::putPheroAntNest(double quantity)
{
  if (quantity > 0)
    m_pheroAntNest += (quantity > 1) ? 1 : quantity;

  m_pheroAntNest = std::min(m_pheroAntNest, 1.);
};

std::ostream &operator<<(std::ostream &out, const Place &place)
{
  out << "{ "
      << "Coord: " << place.getCoord() << ", "
      << "Num Fourmi: " << place.getIdAnt() << ", "
      << "Phero Nid: " << place.getPheroAntNest() << ", "
      << "Phero Sucre: " << place.getPheroSugar() << ", "
      << "Nid: " << (place.isContainingAntNest() ? "Oui" : "Non") << ", "
      << "Sucre: " << (place.isContainingSugar() ? "Oui" : "Non") << " }";

  return out;
}

bool Place::operator==(const Place &p) const
{
  return (m_coord == p.getCoord() and
          m_pheroSugar == p.getPheroSugar() and
          m_pheroAntNest == p.getPheroAntNest() and
          m_idAnt == p.getIdAnt() and
          isContainingSugar() == p.isContainingSugar() and
          isContainingAntNest() == p.isContainingAntNest());
}

void moveAnt(Fourmi &ant, Place &p1, Place &p2)
{
  if (p2.putAnt(ant))
  {
    ant.move(p2.getCoord());
    p1.removeAnt();
  }
}

bool isCloserToNest(Place p1, Place p2)
{
  return p1.getPheroAntNest() > p2.getPheroAntNest();
};

TEST_SUITE_BEGIN("Test de la classe Place.");

TEST_CASE("Test de la fonction putSugar.")
{
  Place p;
  p.putSugar();
  CHECK(p.isContainingSugar());
  CHECK(p.getPheroSugar() == 255);

  Place p2 = Place(Coord(0, 0));
  p2.putAnt(Fourmi(Coord(0, 0), 0));
  CHECK_FALSE(p2.putSugar());
}

TEST_CASE("Test de la fonction removeSugar")
{
  Place p;
  p.putSugar(1);
  p.removeSugar();
  CHECK_FALSE(p.isContainingSugar());
  CHECK(p.getPheroSugar() == 255);
}

TEST_CASE("Test de la fonction putAntNest")
{
  Place p;
  p.putAntNest();
  CHECK(p.isContainingAntNest());
  CHECK(p.getPheroAntNest() == 1);
  CHECK_FALSE(p.putAnt(Fourmi()));
}

TEST_CASE("Test de la fonction putAnt")
{
  // Testes de poser plusieurs fourmis sur une même place.
  Place p;
  p.putAnt(Fourmi(Coord(), 0));
  CHECK(p.isContainingAnt());
  CHECK_FALSE(p.putAnt(Fourmi(Coord(), 1)));

  // Testes de poser un nid et une fourmis sur une même place.
  Place p1;
  p1.putAntNest();
  CHECK_FALSE(p1.putAnt(Fourmi(Coord(), 0)));

  // Testes de poser un sucre et une fourmis sur une même case.
  Place p2;
  p2.putSugar();
  CHECK_FALSE(p2.putAnt(Fourmi(Coord(), 0)));

  // Testes de poser un sucre et une fourmis qui porte un sucre.
  Place p3 = Place(Coord());
  p3.putSugar();
  Fourmi f = Fourmi(Coord(), 0);
  f.carrySugar();
  CHECK(p3.putAnt(f));
  CHECK(p3.isContainingAnt());
}

TEST_CASE("Test de la fonction removeAnt")
{
  Place p;
  p.putAnt(Fourmi(Coord(), 0));
  p.removeAnt();
  CHECK_FALSE(p.isContainingAnt());
}

TEST_CASE("Test de la fonction putPheroAntNest")
{
  Place p;
  p.putPheroAntNest(.5);
  CHECK(p.getPheroAntNest() == .5);
}

TEST_CASE("Test de la fonction putPheroSugar")
{
  Place p;
  p.putPheroSugar(100);
  CHECK(p.getPheroSugar() == 100);
}

TEST_CASE("Test de la fonction removePheroSugar")
{
  Place p;
  p.putPheroSugar(100);
  p.removePheroSugar();
  CHECK_FALSE(p.isContainingPheroSugar());
}

TEST_CASE("Test de la fonction decreasePheroSugar")
{
  Place p;
  p.putPheroSugar(255);
  p.decreasePheroSugar(1);
  CHECK(p.getPheroSugar() == 254);
}

TEST_CASE("Test de l'operator== et !=.")
{
  Place p = Place(Coord(0, 0));
  Place p2 = Place(Coord(1, 0));
  Place p3 = Place(Coord(1, 0));
  p3.putSugar();

  CHECK(p == p);
  CHECK(p != p2);
  CHECK(p2 != p3);
}

TEST_CASE("Test de l'operateur <<.")
{
  std::ostringstream stream;
  stream << Place(Coord(2, 3));

  std::ostringstream streamCheck;
  streamCheck << "{ Coord: (2, 3), "
              << "Num Fourmi: -1, "
              << "Phero Nid: 0, "
              << "Phero Sucre: 0, "
              << "Nid: Non, "
              << "Sucre: Non }";

  CHECK(stream.str() == streamCheck.str());
}

TEST_CASE("Test de la fonction moveAnts.")
{
  Coord c = Coord(0, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(0, 1));
  Fourmi f = Fourmi(c, 1);

  moveAnt(f, p1, p2);
  CHECK_FALSE(p1.isContainingAnt());
  CHECK(p2.isContainingAnt());
  CHECK(f.getCoord() == p2.getCoord());
}

TEST_SUITE_END();
