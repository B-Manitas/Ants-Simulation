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

GrilleFourmis initializeRandomPlaces(Grille &laGrille)
{
  EnsCoord lesSucres = EnsCoord(std::vector<Coord>({Coord(15, 7), Coord(13, 15)}));
  EnsCoord lesNids = EnsCoord(std::vector<Coord>({Coord(9, 9), Coord(9, 10), Coord(10, 9), Coord(10, 10)}));
  EnsCoord lesFourmis = EnsCoord(std::vector<Coord>({Coord(8, 8), Coord(9, 8), Coord(10, 8), Coord(11, 8),
                                                     Coord(8, 9), Coord(11, 9), Coord(8, 10), Coord(11, 10),
                                                     Coord(8, 11), Coord(9, 11)}));

  GrilleFourmis gFourmis = initializeGrid(laGrille, lesNids, lesSucres, lesFourmis);
  testCoherence(laGrille, gFourmis, "Initialisation");

  return gFourmis;
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
      Place p = g.getPlace(c);
      Fourmi f = Fourmi();

      if (lesFourmis.isContainingAntsCoord(c))
        f = lesFourmis.getAnt(c);

      if (p.isContainingAnt() && f.lookForSugar())
        file << " f" << f.getNum();

      else if (p.isContainingAnt())
        file << " F" << f.getNum();

      else if (p.isContainingAntNest())
        file << " n ";

      else if (p.isContainingSugar())
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

void dessinePPM(Grille &laGrille, GrilleFourmis &lesFourmis, int n)
{
  int r, g, b;

  std::ostringstream filename;
  filename << "ppm/img" << std::setfill('0') << std::setw(3) << n << ".ppm";
  std::ofstream fichier(filename.str(), std::ios::out | std::ios::trunc);

  if (not fichier.is_open())
    throw std::runtime_error("Le fichier n'a pas pu être ouvert.");

  // ecriture de l’entete
  fichier << "P3" << std::endl
          << GRID_SIZE << " " << GRID_SIZE << " " << std::endl
          << 255 << " " << std::endl;

  for (int y = 0; y < laGrille.taille(); y++)
  {
    for (int x = 0; x < laGrille.taille(); x++)
    {
      Coord c = Coord(x, y);
      Place p = laGrille.getPlace(c);
      Fourmi f = Fourmi();

      if (lesFourmis.isContainingAntsCoord(c))
        f = lesFourmis.getAnt(c);

      if (p.isContainingAnt() && f.lookForSugar())
        g = 255;

      else if (p.isContainingAntNest())
        b = 255;

      else if (p.isContainingSugar())
      {
        r = 255;
        g = 255;
        b = 255;
      }

      else if (p.getPheroSugar() > 0)
        g = p.getPheroSugar();

      else
      {
        r = 0;
        g = 0;
        b = 0;
      }

      fichier << r << " " << g << " " << b << "    ";
    }

    fichier << std::endl;
  }

  fichier.close();
}

void dessinePheroNid(Grille &g)
{
  for (int y = 0; y < g.taille(); y++)
  {
    std::cout << " | ";
    for (int x = 0; x < g.taille(); x++)
      std::cout << g.m_grille[y][x].getPheroAntNest() << " | ";

    std::cout << std::endl;
  }
}

void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title)
{
  std::ostringstream error_msg;

  for (auto &f : lesFourmis.m_grid)
  {
    Place pF = laGrille.getPlace(f.getCoord());

    if (pF.getIdAnt() != f.getNum())
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
      Place p = laGrille.getPlace(Coord(x, y));

      if (p.isContainingAnt() && not lesFourmis.isContainingAnts(p.getIdAnt()))
      {
        error_msg << title << std::endl;
        error_msg << "Erreur Incoherence: Une place contient une fourmi qui n'existe pas." << std::endl
                  << "Place: " << p << std::endl
                  << "Les Fourmis: " << lesFourmis << std::endl;

        throw std::runtime_error(error_msg.str());
      }
    }
}

// int main(int argc, const char **argv)
// {
//   // srand(time(NULL));
//   doctest::Context context(argc, argv);
//   int test_result = context.run();
//   if (context.shouldExit())
//     return test_result;

//   Grille laGrille = Grille(GRID_SIZE);
//   GrilleFourmis lesFourmis = initializeRandomPlaces(laGrille);
//   GrilleFourmis lesFourmis2 = lesFourmis;
//   int n_iteration = 20;

//   for (int i = 0; i < n_iteration; i++)
//   {
//     dessineGrille(laGrille, lesFourmis, i);
//     dessinePPM(laGrille, lesFourmis, i);
//     updateSetAnts(laGrille, lesFourmis);
//     testCoherence(laGrille, lesFourmis, "Simulation " + std::to_string(i));
//     laGrille.decreasePheroSugar();
//   }

//   if (lesFourmis.m_grid == lesFourmis2.m_grid)
//     std::cout << "equal" << std::endl;

//   return 0;
// }
