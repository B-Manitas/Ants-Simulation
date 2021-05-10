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
#include <sstream>
#include <vector>

std::string repeat_string(std::string str, int n)
{
  std::string new_str = "";
  for (int i = 0; i < n; i++)
    new_str += str;

  return new_str;
}

void initialiserEmplacements(Grille &laGrille, GrilleFourmis &lesFourmis)
{
  EnsCoord centre = EnsCoord();
  int idF = 0;
  for (int y = TAILLEGRILLE / 2 - 2; y < TAILLEGRILLE / 2 + 2; y++)
    for (int x = TAILLEGRILLE / 2 - 2; x < TAILLEGRILLE / 2 + 2; x++)
    {
      Coord c = Coord(x, y);
      centre.ajoute(c);
      if (x >= TAILLEGRILLE / 2 - 1 && x < TAILLEGRILLE / 2 + 1 && y >= TAILLEGRILLE / 2 - 1 && y < TAILLEGRILLE / 2 + 1)
      {
        Place pNid = Place(c);
        pNid.poseNid();
        laGrille.rangePlace(pNid);
      }
      else
      {
        Place pF = Place(c);
        Fourmi f = Fourmi(c, idF);
        pF.poseFourmi(f);
        laGrille.rangePlace(pF);
        lesFourmis.rangeFourmi(f);
        idF++;
      }
    }

  Place pSucre = laGrille.randPlace();

  while (centre.contient(pSucre.getCoord()))
    pSucre = laGrille.randPlace();

  pSucre.poseSucre();
  laGrille.rangePlace(pSucre);
  testCoherence(laGrille, lesFourmis, "Initialisation");
}

void dessineGrille(Grille &g, GrilleFourmis &lesFourmis, int n)
{
  std::ostringstream filename;
  filename << "img/img_" << n << ".txt";

  std::ofstream file(filename.str(), std::ios::out | std::ios::trunc);

  if (not file.is_open())
    throw std::runtime_error("Le fichier n'a pas pu être ouvert.");

  file << "Simulation : " << n << std::endl;

  std::string line = repeat_string("____", g.taille());
  line = "\n" + line + "_";
  for (int y = 0; y < g.taille(); y++)
  {
    file << line << std::endl;

    for (int x = 0; x < g.taille(); x++)
    {
      file << "|";
      Coord c = Coord(x, y);
      Place p = g.chargePlace(c);
      Fourmi f = Fourmi();

      if (lesFourmis.contientFourmisCoord(c))
        f = lesFourmis.chargeFourmi(c);

      if (p.contientFourmi() && f.chercheSucre())
        file << " f ";

      else if (p.contientFourmi())
        file << " F ";

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

void dessine2DVecteur(std::vector<std::vector<Fourmi>> &t)
{
  for (size_t y = 0; y < t.size(); y++)
  {
    std::cout << " | ";

    for (size_t x = 0; x < t.size(); x++)
    {
      std::cout << std::setfill('0') << std::setw(2);
      std::cout << t[y][x].getNum() << " | ";
    }

    std::cout << std::endl;
  }
}

void dessineVecteur(std::vector<Fourmi> &v)
{
  std::cout << "{ ";
  for (auto &f : v)
    std::cout << f << " | ";

  std::cout << " }";
}

void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title)
{
  std::ostringstream error_msg;

  for (auto &f : lesFourmis.m_grilleF)
  {
    Place pF = laGrille.chargePlace(f.getCoord());

    if (pF.getNumeroFourmi() != f.getNum())
    {
      error_msg << title << std::endl;
      error_msg << "Erreur Incoherence: L'indice d'une fourmi ne correspond pas à sa place." << std::endl
                << "Fourmis: " << f << std::endl
                << "Place: " << pF << std::endl;

      throw std::runtime_error(error_msg.str());
    }
  }

  for (int y = 0; y < laGrille.taille(); y++)
    for (int x = 0; x < laGrille.taille(); x++)
    {
      Place p = laGrille.chargePlace(Coord(x, y));

      if (p.contientFourmi() && not lesFourmis.contientFourmis(p.getNumeroFourmi()))
      {
        error_msg << title << std::endl;
        error_msg << "Erreur Incoherence: Une place contient une fourmi qui n'existe pas." << std::endl
                  << "Place: " << p << std::endl
                  << "Les Fourmis: " << lesFourmis << std::endl;

        throw std::runtime_error(error_msg.str());
      }
    }
}

int main(int argc, const char **argv)
{
  // srand(time(NULL));
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  Grille laGrille = Grille(TAILLEGRILLE);
  GrilleFourmis lesFourmis = GrilleFourmis();
  GrilleFourmis lesFourmis2 = lesFourmis;

  initialiserEmplacements(laGrille, lesFourmis);
  // std::cout << "Les Fourmis: " << lesFourmis << std::endl;
  int n_iteration = 2;

  for (int i = 0; i < n_iteration; i++)
  {
    dessineGrille(laGrille, lesFourmis, i);
    // dessineVecteur(lesFourmis.m_grilleF);

    std::cout << "\nSimu: " << i << std::endl;
    mettreAJourEnsFourmis(laGrille, lesFourmis);
    testCoherence(laGrille, lesFourmis, "Simulation " + std::to_string(i));
    std::cout << "\nFin" << std::endl;

    // if (lesFourmis.m_grilleF == lesFourmis2.m_grilleF)
    //   std::cout << "equal" << std::endl;
  }

  return 0;
}
