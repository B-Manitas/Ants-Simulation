#include "grilleFourmis.hpp"
#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"

GrilleFourmis::GrilleFourmis(int taille)
{
  for (int y = 0; y < taille; y++)
  {
    m_grilleF.push_back(std::vector<Fourmi>());

    for (int x = 0; x < taille; x++)
      m_grilleF[y].push_back(Fourmi());
  }
}

Fourmi GrilleFourmis::chargeFourmi(Coord c)
{
  int cX = c.getX();
  int cY = c.getY();

  if (cX >= 0 && cX < taille() && cY >= 0 && cY < taille())
    return m_grilleF[cY][cX];

  else
    throw std::runtime_error("La coordonnées est en dehors du tableau.");
}

void GrilleFourmis::rangeFourmi(Fourmi f)
{
  m_grilleF[f.getCoord().getY()][f.getCoord().getX()] = f;
}

Fourmi GrilleFourmis::chercheFourmi(int idFourmi)
{
  for (int y = 0; y < taille(); y++)
    for (int x = 0; x < taille(); x++)
      if (m_grilleF[y][x].getNum() == idFourmi)
        return m_grilleF[y][x];

  throw std::runtime_error("Aucune fourmis possède n'a été trouvé avec ce numéro.");
}

TEST_SUITE_BEGIN("Test de la classe GrilleFourmis.");
TEST_CASE("Test des méthodes de la classe GrilleFOurmis.")
{
  Fourmi f = Fourmi(Coord(0, 0), 1);
  GrilleFourmis g = GrilleFourmis(2);
  g.rangeFourmi(f);
  CHECK(g.chargeFourmi(f.getCoord()).getNum() == f.getNum());
  CHECK(g.chercheFourmi(1).getNum() == f.getNum());
}
TEST_SUITE_END();
