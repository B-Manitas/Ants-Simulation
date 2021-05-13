#include "grilleFourmis.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"

// GrilleFourmis::GrilleFourmis(int taille) : m_grilleF{std::vector<Fourmi>(Fourmi(Coord(), -1), taille)} {}

bool GrilleFourmis::contientFourmis(int idFourmi)
{
  for (auto &f : m_grilleF)
    if (f.getNum() == idFourmi)
      return true;

  return false;
}

bool GrilleFourmis::contientFourmisCoord(Coord c)
{
  for (auto &f : m_grilleF)
    if (f.getCoord() == c)
      return true;

  return false;
}

// void GrilleFourmis::supprime(Coord c)
// {
//   if (contientFourmisCoord(c))
//     m_grilleF.erase(m_grilleF.begin() + position(c));

//   else
//     throw std::runtime_error("La coordonnée n'a pas pu être supprimée.");
// }

int GrilleFourmis::position(int const &idFourmi)
{
  for (int i = 0; i < taille(); i++)
    if (m_grilleF[i].getNum() == idFourmi)
      return i;

  return -1;
}

Fourmi GrilleFourmis::chargeFourmi(Coord c)
{
  for (auto &f : m_grilleF)
    if (f.getCoord() == c)
      return f;

  throw std::runtime_error("Aucune fourmi n'a été trouvé.");
}

void GrilleFourmis::rangeFourmi(Fourmi &fourmi)
{
  if (not contientFourmis(fourmi.getNum()))
    m_grilleF.push_back(fourmi);

  else
    m_grilleF[position(fourmi.getNum())] = fourmi;
}

Fourmi GrilleFourmis::chercheFourmi(int idFourmi)
{
  for (int i = 0; i < taille(); i++)
    if (m_grilleF[i].getNum() == idFourmi)
      return m_grilleF[i];

  throw std::runtime_error("Aucune fourmis n'a été trouvé avec ce numéro.");
}

std::ostream &operator<<(std::ostream &out, GrilleFourmis &grille)
{
  out << "{";
  for (int i = 0; i < grille.taille(); i++)
  {
    out << grille.m_grilleF[i];
    if (i != grille.taille() - 1)
      out << ", ";
  }
  out << "}";

  return out;
}

TEST_SUITE_BEGIN("Test de la classe GrilleFourmis.");
TEST_CASE("Test des méthodes de la classe GrilleFourmis.")
{
  Fourmi f = Fourmi(Coord(0, 0), 1);
  Fourmi f2 = Fourmi(Coord(1, 0), 2);
  GrilleFourmis g = GrilleFourmis();
  g.rangeFourmi(f);
  g.rangeFourmi(f2);
  CHECK(g.chargeFourmi(f.getCoord()).getNum() == f.getNum());
  CHECK(g.chercheFourmi(1).getNum() == f.getNum());
  CHECK(g.taille() == 2);
}
TEST_SUITE_END();
