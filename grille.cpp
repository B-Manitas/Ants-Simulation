#include "grille.hpp"
#include "place.hpp"
#include "coord.hpp"

Place Grille::chargePlace(Coord c)
{
  return grille[c.getLig()][c.getCol()];
}

void Grille::rangePlace(Place p)
{
  grille[p.getCoord().getLig()][p.getCoord().getCol()] = p;
}

void Grille::linearisePheroNid()
{
  bool stable = false;

  while (not stable)
  {
    stable = true;
    for (int i = 0; i < grille.size(); i++)
    {
      for (int j = 0; j < grille[0].size(); j++)
      {
        Coord c = Coord(i, j);
        Place p = chargePlace(c);
        if (p.getPheroNid() < 1)
        {
          EnsCoord coordVoisins = voisines(c);
          float maxPhero = 0;

          for (int pos = 0; i < coordVoisins.taille(); pos++)
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
  for (int i = 0; i < grille.size(); i++)
  {
    for (int j = 0; j < grille[0].size(); j++)
    {
      Coord c = Coord(i, j);
      Place p = chargePlace(c);
      p.diminuerPheroSucre();
      rangePlace(p);
    }
  }
}