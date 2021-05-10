#include "place.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"
#include <sstream>

void Place::poseSucre()
{
  if (not contientFourmi())
    m_contientSucre = true;
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
  if (not contientFourmi() && (not contientSucre() || fourmi.chercheSucre()) && not contientNid())
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
  return (m_coord == p.getCoord() and
          m_pheroSucre == p.getPheroSucre() and
          m_pheroNid == p.getPheroNid() and
          m_numeroFourmi == p.getNumeroFourmi() and
          m_contientSucre == p.contientSucre());
}

std::ostream &operator<<(std::ostream &out, const Place &place)
{
  out << "{ "
      << "Coord: " << place.getCoord() << ", "
      << "Num Fourmi: " << place.getNumeroFourmi() << ", "
      << "Phero Nid: " << place.getPheroNid() << ", "
      << "Phero Sucre: " << place.getPheroSucre() << " }";
  return out;
}

void deplaceFourmi(Fourmi &fourmi, Place &p1, Place &p2)
{
  p1.enleveFourmi();
  fourmi.deplace(p2.getCoord());
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
  CHECK(p.contientSucre());
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

TEST_CASE("Test de l'operateur <<.")
{
  std::ostringstream stream;
  stream << Place(Coord(2, 3));

  std::ostringstream streamCheck;
  streamCheck << "{ Coord: (2, 3), "
              << "Num Fourmi: -1, "
              << "Phero Nid: 0, "
              << "Phero Sucre: 0 }";

  CHECK(stream.str() == streamCheck.str());
}

TEST_CASE("Test de la fonction deplaceFourmis.")
{
  Coord c = Coord(0, 0);
  Place p1 = Place(c);
  Place p2 = Place(Coord(0, 1));
  Fourmi f = Fourmi(c, 1);

  deplaceFourmi(f, p1, p2);
  CHECK_FALSE(p1.contientFourmi());
  CHECK(p2.contientFourmi());
  CHECK(f.getCoord() == p2.getCoord());
}

TEST_SUITE_END();
