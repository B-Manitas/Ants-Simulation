#include "place.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"
#include <sstream>

void Place::poseSucre()
{
  if (not contientFourmi())
    m_pheroSucre = 255;
};

void Place::poseNid()
{
  if (not contientFourmi())
    m_pheroNid = 1;
};

void Place::posePheroSucre(double quantite)
{
  if (quantite > 0)
    m_pheroSucre += (quantite > 255) ? 255 : quantite;

  m_pheroSucre = std::min(m_pheroSucre, 255);
};

void Place::posePheroNid(double quantite)
{
  if (quantite > 0)
    m_pheroNid += (quantite > 1) ? 1 : quantite;

  m_pheroNid = std::min(m_pheroNid, 1.);
};

void Place::poseFourmi(Fourmi fourmi)
{
  if (not contientFourmi() && (not contientSucre() || fourmi.getContientSucre()) && not contientNid())
  {
    m_numeroFourmi = fourmi.getNum();
  }
};

void Place::enleveFourmi()
{
  m_numeroFourmi = -1;
};

void Place::diminuerPheroSucre()
{
  if (m_pheroSucre > 0)
    m_pheroSucre -= 5;
};

bool Place::operator==(const Place &p) const
{
  return ((m_coord == p.getCoord()) && (m_pheroSucre == p.getPheroSucre()) && (m_pheroNid == p.getPheroNid()) && (m_numeroFourmi == getNumeroFourmi()));
}

std::ostream &operator<<(std::ostream &out, const Place &place)
{
  return out << place.getCoord();
}

void deplaceFourmi(Fourmi fourmi, Place p1, Place p2)
{
  p1.enleveFourmi();
  p2.poseFourmi(fourmi);
}

bool estPlusProcheNid(Place p1, Place p2)
{
  return p1.getPheroNid() > p2.getPheroNid();
};

TEST_SUITE_BEGIN("Test de la classe Place.");

TEST_CASE("Test des méthodes de la classe Place.")
{
  Coord c = Coord(0, 0);
  Place p = Place(Coord(0, 0));
  CHECK_FALSE(p.contientSucre());
  CHECK_FALSE(p.contientNid());
  CHECK_FALSE(p.contientFourmi());
  CHECK_FALSE(p.estSurUnePiste());
  CHECK(p.estVide());
  p.poseFourmi(Fourmi(c, 0));
  CHECK(p.contientFourmi());
  CHECK(p.getNumeroFourmi() == 0);
  p.enleveFourmi();
  p.poseSucre();
  CHECK(p.getPheroSucre() == 255);
  p.enleveSucre();
  CHECK(p.getPheroSucre() == 0);
  p.poseNid();
  CHECK(p.getPheroNid() == 1);
  p.poseFourmi(Fourmi(c, 0));
  CHECK_FALSE(p.contientFourmi());
  p.enleveSucre();
  CHECK_FALSE(p.contientFourmi());
  p.posePheroNid(2);
  CHECK(p.getPheroNid() == 1);
  p.posePheroSucre(300);
  CHECK(p.getPheroSucre() == 255);
  p.diminuerPheroSucre();
  CHECK(p.getPheroSucre() == 250);
}

TEST_CASE("Test de l'operator==.")
{
  Place p = Place(Coord(0, 0));
  Place p2 = Place(Coord(1, 0));
  Place p3 = Place(Coord(1, 0));
  p3.poseSucre();

  CHECK(p == p);
  CHECK_FALSE(p == p2);
  CHECK_FALSE(p2 == p3);
}

TEST_CASE("Test de l'operateur <<")
{
  std::ostringstream stream;
  stream << Place(Coord(2, 3));
  CHECK(stream.str() == "(2, 3)");
}

TEST_SUITE_END();
