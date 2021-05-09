#include "grille.hpp"
#include "place.hpp"
#include "coord.hpp"
#include "fourmi.hpp"
#include "action.hpp"
#include "doctest.h"
#include <vector>

Grille::Grille(int taille)
{
  for (int y = 0; y < taille; y++)
  {
    m_grille.push_back(std::vector<Place>());

    for (int x = 0; x < taille; x++)
    {
      m_grille[y].push_back(Place(Coord(x, y)));
    }
  }
}

std::ostream &operator<<(std::ostream &out, const Grille &g)
{
  out << "{";
  for (int i = 0; i < g.m_grille.size(); i++)
  {
    for (int j = 0; j < g.m_grille.size(); j++)
    {
      out << g.m_grille[i][j];
    }
    out << std::endl;
  }
  out << "}";
  return out;
}

Place Grille::chargePlace(const Coord c)
{
  int cX = c.getX();
  int cY = c.getY();

  if (cX >= 0 && cX < m_grille[0].size() && cY >= 0 && cY < m_grille.size())
    return m_grille[cY][cX];

  else
    throw std::runtime_error("La coordonnées est en dehors du tableau.");
}

void Grille::rangePlace(Place p)
{
  m_grille[p.getCoord().getY()][p.getCoord().getX()] = p;
}

void Grille::linearisePheroNid()
{
  bool stable = false;

  while (not stable)
  {
    stable = true;
    for (int y = 0; y < taille(); y++)
    {
      for (int x = 0; x < taille(); x++)
      {
        Coord c = Coord(x, y);
        Place p = chargePlace(c);
        if (p.getPheroNid() < 1)
        {
          EnsCoord coordVoisins = voisines(c);
          double maxPhero = 0;

          for (int pos = 0; y < coordVoisins.taille(); pos++)
          {
            Place v = chargePlace(coordVoisins.ieme(pos));
            maxPhero = std::max(maxPhero, v.getPheroNid());
          }

          maxPhero -= 1. / TAILLEGRILLE;

          if (maxPhero > p.getPheroNid())
          {
            p.posePheroSucre(maxPhero);
            rangePlace(p);
            stable = false;
          }
        }
      }
    }
  }
}

void Grille::diminuePheroSucre()
{
  for (int y = 0; y < taille(); y++)
    for (int x = 0; x < taille(); x++)
    {
      Coord c = Coord(x, y);
      Place p = chargePlace(c);
      p.diminuerPheroSucre();
      rangePlace(p);
    }
}

void mettreAJourUneFourmi(Fourmi fourmi, Grille laGrille)
{
  Coord coordF = fourmi.getCoord();
  Place pf = laGrille.chargePlace(coordF);
  EnsCoord voisCoord = voisines(coordF);

  for (int numRegle = 0; numRegle < 5; numRegle++)
    for (int i = 0; i < voisCoord.taille(); i++)
    {
      Place vois = laGrille.chargePlace(voisCoord.ieme(i));
      if (Action().condtion_n(numRegle, fourmi, pf, vois))
      {
        Action().action_n(numRegle, fourmi, pf, vois);
        laGrille.rangePlace(pf);
        laGrille.rangePlace(vois);
        return;
      }
    }
}

void mettreAJourEnsFourmis(Grille laGrille, std::vector<std::vector<Fourmi>> lesFourmis)
{

  for (int y = 9; y < lesFourmis.size(); y++)
    for (int x = 9; x < lesFourmis[y].size(); x++)
      mettreAJourUneFourmi(lesFourmis[y][x], laGrille);
}

TEST_SUITE_BEGIN("Test de la classe Grille.");
TEST_CASE("Test de la fonction chargePlace.")
{
  CHECK(Grille(1).chargePlace(Coord(0, 0)) == Place(Coord(0, 0)));
  CHECK(Grille(5).chargePlace(Coord(4, 4)) == Place(Coord(4, 4)));
  CHECK_THROWS_AS(Grille(5).chargePlace(Coord(5, 4)), std::runtime_error);
  CHECK_THROWS_AS(Grille(5).chargePlace(Coord(4, 5)), std::runtime_error);
}

TEST_CASE("Test de la fonction chargePlace.")
{
  Grille g = Grille(2);
  Coord c = Coord(1, 0);
  Place p = g.chargePlace(c);
  p.poseSucre();
  g.rangePlace(p);

  CHECK(g.chargePlace(c) == p);
}
TEST_SUITE_END();
