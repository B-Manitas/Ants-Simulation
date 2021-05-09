#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "principal.hpp"
#include "grille.hpp"
#include "coord.hpp"
#include "place.hpp"
#include "fourmi.hpp"
#include "grilleFourmis.hpp"
#include <iostream>
#include <fstream>
#include <string>

Grille initialiserEmplacements()
{
  Grille laGrille = Grille(TAILLEGRILLE);
  int idF = 0;
  for (int y = 9; y < 13; y++)
    for (int x = 9; x < 13; x++)
    {
      Coord c = Coord(x, y);

      if (x >= 10 && x <= 12 && y >= 10 && y <= 12)
      {
        Place pNid = Place(c);
        pNid.poseNid();
        laGrille.rangePlace(pNid);
      }
      else
      {
        Place pF = Place(c);
        pF.poseFourmi(Fourmi(c, idF));
        laGrille.rangePlace(pF);
      }
    }

  return laGrille;
}

void dessineGrille(Grille g, GrilleFourmis lesFourmis, int n)
{
  std::string line = std::string("_", g.taille());
  std::cout << line << std::endl;

  std::ofstream file;
  file.open("output.txt", std::fstream::out | std::fstream::trunc);
  if (not file.is_open())
    throw std::runtime_error("Le fichier n'a pas pu être ouvert.");

  file << line << std::endl;
  file << "Simulation : " << n << std::endl;
  file << line << std::endl;

  for (int y = 9; y < g.taille(); y++)
  {
    file << line << std::endl;

    for (int x = 9; x < g.taille(); x++)
    {
      file << "|";
      Coord c = Coord(x, y);
      Place p = g.chargePlace(c);
      Fourmi f = lesFourmis.chargeFourmi(c);

      if (p.contientNid() && f.getContientSucre())
        file << " F ";

      else if (p.contientFourmi())
        file << " f ";

      else if (p.contientNid())
        file << " n ";

      else if (p.contientSucre())
        file << " s ";

      else
        file << "   ";

      if (x == g.taille() - 1)
        file << "|";
    }

    if (y == g.taille() - 1)
      file << line << std::endl;
  }

  file.close();
}

int main(int argc, const char **argv)
{
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  Grille laGrille = initialiserEmplacements();
  GrilleFourmis lesFourmis = GrilleFourmis(TAILLEGRILLE);
  dessineGrille(laGrille, lesFourmis, 1);

  return 0;
}
