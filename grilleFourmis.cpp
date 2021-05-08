#include "doctest.h"
#include "fourmi.hpp"
#include "grilleFourmis.hpp"
#include "coord.hpp"

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

TEST_CASE("Test des méthodes de la classe GrilleFOurmis.")
{
  Fourmi f = Fourmi(Coord(0, 0), 1);
  GrilleFourmis g = GrilleFourmis(2);
  g.rangeFourmi(f);
  CHECK(g.chargeFourmi(f.getCoord()).getNum() == f.getNum());
  CHECK(g.chercheFourmi(1).getNum() == f.getNum());
}
