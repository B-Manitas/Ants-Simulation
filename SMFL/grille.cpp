#include "grille.hpp"
#include "place.hpp"
#include "coord.hpp"
#include "fourmi.hpp"
#include "action.hpp"
#include "grilleFourmis.hpp"
#include "doctest.h"
#include <vector>
#include <iostream>

Grille::Grille(int const taille)
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
    throw std::runtime_error("La coordonnée est en dehors de la grille.");
}

void Grille::rangePlace(Place &p)
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
      for (int x = 0; x < taille(); x++)
      {
        Coord c = Coord(x, y);
        Place p = chargePlace(c);

        if (p.getPheroNid() < 1)
        {
          EnsCoord coordVoisins = voisines(c);
          double maxPhero = 0;

          for (int posVois = 0; posVois < coordVoisins.taille(); posVois++)
          {
            Place v = chargePlace(coordVoisins.ieme(posVois));
            maxPhero = std::max(maxPhero, v.getPheroNid());
          }

          maxPhero -= 1. / TAILLEGRILLE;

          if (maxPhero > p.getPheroNid())
          {
            p.posePheroNid(maxPhero);
            rangePlace(p);
            stable = false;
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

Place Grille::randPlace()
{
  return chargePlace(Coord(std::rand() % taille(), std::rand() % taille()));
}

void placeNids(Grille &g, EnsCoord ens)
{
  for (int i = 0; i < ens.taille(); i++)
  {
    Place p = g.chargePlace(ens.ieme(i));
    p.poseNid();
    g.rangePlace(p);
  }
}

void placeSucres(Grille &g, EnsCoord ens)
{
  for (int i = 0; i < ens.taille(); i++)
  {
    Place p = g.chargePlace(ens.ieme(i));
    p.poseSucre();
    g.rangePlace(p);
  }
}

GrilleFourmis placeFourmis(Grille &g, EnsCoord ens)
{
  GrilleFourmis lesFourmis;
  for (int i = 0; i < ens.taille(); i++)
  {
    Coord c = ens.ieme(i);
    Place p = g.chargePlace(c);
    Fourmi f = Fourmi(c, i);
    lesFourmis.rangeFourmi(f);
    p.poseFourmi(f);
    g.rangePlace(p);
  }

  return lesFourmis;
}

GrilleFourmis initialiseGrille(Grille &g, EnsCoord lesNids, EnsCoord lesSucres, EnsCoord lesFourmis)
{
  placeNids(g, lesNids);
  placeSucres(g, lesSucres);
  GrilleFourmis gFourmis = placeFourmis(g, lesFourmis);

  return gFourmis;
}

void mettreAJourUneFourmi(Fourmi f, Grille &laGrille, GrilleFourmis &lesFourmis)
{
  Coord c = f.getCoord();
  Place p1 = laGrille.chargePlace(c);
  EnsCoord voisCoord = voisines(c);

  std::vector<int> action_i = std::vector<int>{2, 3, 4, 7};
  for (auto &numR : action_i)
  {
    Coord cv = voisCoord.ieme(rand() % voisCoord.taille());
    voisCoord.supprime(cv);
    Place p2 = laGrille.chargePlace(cv);

    if (Action().condtion_n(numR, f, p1, p2))
    {
      // std::cout << numR << " ";
      Action().action_n(numR, f, p1, p2);
      laGrille.rangePlace(p1);
      laGrille.rangePlace(p2);
      lesFourmis.rangeFourmi(f);
      return;
    }
    // for (int i = 0; i < voisCoord.taille(); i++)
    // {
    // }
  }
}

void mettreAJourEnsFourmis(Grille &laGrille, GrilleFourmis &lesFourmis)
{
  for (auto &f : lesFourmis.m_grilleF)
    mettreAJourUneFourmi(f, laGrille, lesFourmis);
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
