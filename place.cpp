#include "place.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"
#include <sstream>

bool Place::poseSucre()
{
  if (not contientFourmi())
  {
    m_morceauSucre++;
    posePheroSucre(255);
    return true;
  }

  return false;
};

void Place::enleveSucre()
{
  if (contientSucre())
    m_morceauSucre--;
};

bool Place::poseNid()
{
  if (not contientFourmi())
  {
    m_morceauNid++;
    posePheroNid(1);
    return true;
  }

  return false;
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

bool Place::poseFourmi(Fourmi fourmi)
{
  if (not contientFourmi() && (not contientSucre() || (contientSucre() && not fourmi.chercheSucre())) && not contientNid())
  {
    m_numeroFourmi = fourmi.getNum();
    return true;
  }

  return false;
  // else
  // {
  //   std::ostringstream error_msg;
  //   error_msg << "Une fourmi n'a pas pû être poser." << std::endl
  //             << "Condition 1: " << not contientFourmi() << std::endl
  //             << "Condition 2: " << (not contientSucre() || fourmi.chercheSucre()) << std::endl
  //             << "Condition 3: " << not contientNid() << std::endl
  //             << "La fourmi: " << fourmi << std::endl
  //             << "La place: " << *this << std::endl;

  //   throw std::runtime_error(error_msg.str());
  // }
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
          contientSucre() == p.contientSucre() and
          contientNid() == p.contientNid());
}

std::ostream &operator<<(std::ostream &out, const Place &place)
{
  out << "{ "
      << "Coord: " << place.getCoord() << ", "
      << "Num Fourmi: " << place.getNumeroFourmi() << ", "
      << "Phero Nid: " << place.getPheroNid() << ", "
      << "Phero Sucre: " << place.getPheroSucre() << ", "
      << "Nid: " << (place.contientNid() ? "Oui" : "Non") << ", "
      << "Sucre: " << (place.contientSucre() ? "Oui" : "Non") << " }";

  return out;
}

void deplaceFourmi(Fourmi &fourmi, Place &p1, Place &p2)
{
  if (p2.poseFourmi(fourmi))
  {
    fourmi.deplace(p2.getCoord());
    p1.enleveFourmi();
  }
}

bool estPlusProcheNid(Place p1, Place p2)
{
  return p1.getPheroNid() > p2.getPheroNid();
};

TEST_SUITE_BEGIN("Test de la classe Place.");

TEST_CASE("Test de la fonction poseSucre.")
{
  Place p;
  p.poseSucre();
  CHECK(p.contientSucre());
  CHECK(p.getPheroSucre() == 255);

  Place p2 = Place(Coord(0, 0));
  p2.poseFourmi(Fourmi(Coord(0, 0), 0));
  CHECK_FALSE(p2.poseSucre());
}

TEST_CASE("Test de la fonction enleveSucre")
{
  Place p;
  p.poseSucre();
  p.enleveSucre();
  CHECK_FALSE(p.contientSucre());
  CHECK(p.getPheroSucre() == 255);
}

TEST_CASE("Test de la fonction poseNid")
{
  Place p;
  p.poseNid();
  CHECK(p.contientNid());
  CHECK(p.getPheroNid() == 1);
  CHECK_FALSE(p.poseFourmi(Fourmi()));
}

TEST_CASE("Test de la fonction poseFourmi")
{
  // Testes de poser plusieurs fourmis sur une même place.
  Place p;
  p.poseFourmi(Fourmi(Coord(), 0));
  CHECK(p.contientFourmi());
  CHECK_FALSE(p.poseFourmi(Fourmi(Coord(), 1)));

  // Testes de poser un nid et une fourmis sur une même place.
  Place p1;
  p1.poseNid();
  CHECK_FALSE(p1.poseFourmi(Fourmi(Coord(), 0)));

  // Testes de poser un sucre et une fourmis sur une même case.
  Place p2;
  p2.poseSucre();
  CHECK_FALSE(p2.poseFourmi(Fourmi(Coord(), 0)));

  // Testes de poser un sucre et une fourmis qui porte un sucre.
  Place p3 = Place(Coord());
  p3.poseSucre();
  Fourmi f = Fourmi(Coord(), 0);
  f.porteSucre();
  CHECK(p3.poseFourmi(f));
  CHECK(p3.contientFourmi());
}

TEST_CASE("Test de la fonction enleveFourmi")
{
  Place p;
  p.poseFourmi(Fourmi(Coord(), 0));
  p.enleveFourmi();
  CHECK_FALSE(p.contientFourmi());
}

TEST_CASE("Test de la fonction posePheroNid")
{
  Place p;
  p.posePheroNid(.5);
  CHECK(p.getPheroNid() == .5);
}

TEST_CASE("Test de la fonction posePheroSucre")
{
  Place p;
  p.posePheroSucre(100);
  CHECK(p.getPheroSucre() == 100);
}

TEST_CASE("Test de la fonction diminuePheroSucre")
{
  Place p;
  p.posePheroSucre(255);
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
              << "Phero Sucre: 0, "
              << "Nid: Non, "
              << "Sucre: Non }";

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
