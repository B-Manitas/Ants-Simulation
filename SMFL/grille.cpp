#include "grille.hpp"
#include "action.hpp"
#include "place.hpp"
#include "coord.hpp"
#include "fourmi.hpp"
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

void placeFourmis(Grille &g, GrilleFourmis &lesFourmis, EnsCoord ens)
{
  for (int i = 0; i < ens.taille(); i++)
  {
    Coord c = ens.ieme(i);
    Place p = g.chargePlace(c);
    Fourmi f = Fourmi(c, i);
    lesFourmis.rangeFourmi(f);
    p.poseFourmi(f);
    g.rangePlace(p);
  }
}

void initialiseGrille(Grille &g, GrilleFourmis &lesFourmisGrille, EnsCoord lesNids, EnsCoord lesSucres, EnsCoord lesFourmis)
{
  placeNids(g, lesNids);
  placeSucres(g, lesSucres);
  placeFourmis(g, lesFourmisGrille, lesFourmis);
}

void initialiserEmplacements(Grille &laGrille, GrilleFourmis &lesFourmisGrille)
{
  int n_sucre = 10;
  int n_nid = 1;
  int n_fourmi = 20;

  EnsCoord ens;
  EnsCoord ens_nid;
  EnsCoord ens_nid_voisine;
  EnsCoord ens_sucres;
  EnsCoord ens_fourmi;
  for (int i = 0; i < n_sucre; i++)
  {
    Coord rand_coord = laGrille.randPlace().getCoord();
    if (not ens.contient(rand_coord))
    {
      ens.ajoute(rand_coord);
      ens_sucres.ajoute(rand_coord);
    }
  }

  Coord rand_coord = laGrille.randPlace().getCoord();
  if (not ens.contient(rand_coord))
  {
    ens.ajoute(rand_coord);
    ens_nid.ajoute(rand_coord);

    EnsCoord voisines_coord = voisines(rand_coord);
    for (int i = 0; i < voisines_coord.taille(); i++)
      ens_nid_voisine.ajoute(voisines_coord.ieme(i));
  }

  if (n_fourmi > ens_nid_voisine.taille())
    n_fourmi = ens_nid_voisine.taille();

  for (int i = 0; i < n_fourmi; i++)
  {
    Coord rand_coord = choixHasard(ens_nid_voisine);
    ens_fourmi.ajoute(rand_coord);
    ens_nid_voisine.supprime(rand_coord);
  }

  initialiseGrille(laGrille, lesFourmisGrille, ens_nid, ens_sucres, ens_fourmi);
  laGrille.linearisePheroNid();
}

void mettreAJourUneFourmi(Fourmi &f, Grille &laGrille)
{
  Coord c = f.getCoord();
  Place p1 = laGrille.chargePlace(c);
  EnsCoord voisCoord = voisines(c);
  voisCoord.melange();

  for (int i = 0; i < voisCoord.taille(); i++)
  {
    Coord cv = voisCoord.ieme(i);
    Place p2 = laGrille.chargePlace(cv);

    for (int num_r = 2; num_r < 8; num_r++)
    {
      if (condtion_n(num_r, f, p1, p2))
      {
        action_n(num_r, f, p1, p2);
        laGrille.rangePlace(p1);
        laGrille.rangePlace(p2);
        return;
      }
    }
  }
}

void mettreAJourEnsFourmis(Grille &laGrille, GrilleFourmis &lesFourmis)
{
  for (int i = 0; i < lesFourmis.taille(); i++)
    mettreAJourUneFourmi(lesFourmis.m_grilleF[i], laGrille);
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
