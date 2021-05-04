#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "doctest.h"
#include "coord.hpp"

// Définition des méthodes de la classe Coord.
Coord::Coord(int lig, int col) : lig{lig}, col{col}
{
  if (lig < 0 or col < 0 or lig > TAILLEGRILLE or col > TAILLEGRILLE)
    throw std::invalid_argument("La coordonnées est en dehors de la grille.");
}

TEST_CASE("Test du constructeur Coord")
{
  CHECK_THROWS_AS(Coord(-5, -5), std::invalid_argument);
  CHECK_THROWS_AS(Coord(-5, TAILLEGRILLE), std::invalid_argument);
  CHECK_THROWS_AS(Coord(TAILLEGRILLE + 5, TAILLEGRILLE + 5), std::invalid_argument);
}

int Coord::getLig() const
{
  return lig;
}

int Coord::getCol() const
{
  return col;
}

Coord Coord::getCoord() const
{
  return {lig, col};
}

TEST_CASE("Test du getter.")
{
  Coord A = Coord{2, 3};
  CHECK(A.getCoord() == Coord{2, 3});
  CHECK(A.getLig() == 2);
  CHECK(A.getCol() == 3);
}

std::ostream &operator<<(std::ostream &out, const Coord &coord)
{
  return out << "(" << coord.getLig() << ", " << coord.getCol() << ")";
}

TEST_CASE("Test de l'operateur <<")
{
  std::ostringstream stream;
  stream << Coord(2, 3);
  CHECK(stream.str() == "(2, 3)");
}

bool Coord::operator==(const Coord &coord) const
{
  return lig == coord.getLig() && col == coord.getCol();
}

TEST_CASE("Test de l'operateur ==")
{
  CHECK(Coord{2, 3} == Coord{2, 3});
  CHECK_FALSE(Coord{2, 3} == Coord{3, 2});
}

bool Coord::operator!=(const Coord &coord) const
{
  return !(*this == coord);
}

TEST_CASE("Test de l'operateur !=")
{
  CHECK(Coord{2, 3} != Coord{3, 2});
  CHECK_FALSE(Coord{2, 3} != Coord{2, 3});
}

// Définition des méthodes de la classe EnsCoord.

std::ostream &operator<<(std::ostream &out, const EnsCoord &e)
{
  for (auto &val : e.coords)
    out << val << ", ";

  return out;
}

TEST_CASE("Test de l'operateur << ")
{
  std::ostringstream stream;
  std::vector<Coord> v1 = std::vector<Coord>({Coord(1, 2)});
  std::vector<Coord> v2 = std::vector<Coord>({Coord(1, 2), Coord(3, 4)});

  stream << EnsCoord(v1);
  CHECK(stream.str() == "(1, 2), ");

  stream.str("");
  stream << EnsCoord(v2);
  CHECK(stream.str() == "(1, 2), (3, 4), ");
}

int EnsCoord::position(const Coord &c) const
{
  for (int i = 0; i < coords.size(); i++)
  {
    if (coords[i] == c)
      return i;
  }

  return -1;
}

bool EnsCoord::contient(const Coord &c) const
{
  return position(c) != -1;
}

TEST_CASE("Test de la méthode contient.")
{
  std::vector<Coord> v = std::vector<Coord>({Coord(1, 2), Coord(3, 4)});

  CHECK(EnsCoord(v).contient(Coord(1, 2)));
  CHECK(EnsCoord(v).contient(Coord(3, 4)));
  CHECK_FALSE(EnsCoord(v).contient(Coord(5, 2)));
}

bool EnsCoord::operator==(const EnsCoord &ens) const
{
  for (int i = 0; i < taille(); i++)
    if (not contient(ens.ieme(i)))
      return false;

  return taille() && ens.taille();
}

TEST_CASE("Test de l'opérateur ==")
{
  EnsCoord ens1 = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 4)}));
  EnsCoord ens2 = EnsCoord(std::vector<Coord>({Coord(3, 4), Coord(1, 2)}));
  EnsCoord ens3 = EnsCoord(std::vector<Coord>({Coord(2, 3)}));
  CHECK(ens1 == ens1);
  CHECK(ens1 == ens2);
  CHECK_FALSE(ens1 == ens3);
}

void EnsCoord::ajoute(const Coord c)
{
  if (not contient(c))
    coords.push_back(c);
}

TEST_CASE("Test de la méthode ajoute.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>());
  ens.ajoute(Coord(1, 2));
  CHECK(ens.contient(Coord(1, 2)));

  ens.ajoute(Coord(3, 2));
  CHECK(ens.contient(Coord(3, 2)));
}

void EnsCoord::supprime(const Coord c)
{
  if (contient(c))
    coords.erase(coords.begin() + position(c));

  else
    throw std::runtime_error("La coordonnée n'a pas pu être supprimée.");
}

TEST_CASE("Test de la méthode supprime.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)}));

  ens.supprime(Coord(1, 2));
  CHECK(not ens.contient(Coord(1, 2)));

  ens.supprime(Coord(3, 2));
  CHECK(not ens.contient(Coord(3, 2)));
  CHECK_THROWS_AS(ens.supprime(Coord(3, 2)), std::runtime_error);
}

bool EnsCoord::estVide() const
{
  return coords.size() == 0;
}

TEST_CASE("Test de la méthode estVide.")
{
  CHECK(EnsCoord(std::vector<Coord>()).estVide());
  CHECK(not EnsCoord(std::vector<Coord>({Coord(1, 2)})).estVide());
}

int EnsCoord::taille() const
{
  return coords.size();
}

TEST_CASE("Test de la méthode taille.")
{
  CHECK(EnsCoord(std::vector<Coord>()).taille() == 0);
  CHECK(EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)})).taille() == 2);
}

Coord EnsCoord::ieme(const int &n) const
{
  if (n >= 0 and n < taille())
    return coords[n];

  else
    throw std::invalid_argument("Le paramètre n est en dehors des bornes de l'ensemble.");
}

TEST_CASE("Test de la méthode ieme.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)}));
  CHECK(ens.ieme(0) == Coord(1, 2));
  CHECK(ens.ieme(1) == Coord(3, 2));
  CHECK_THROWS_AS(ens.ieme(2), std::invalid_argument);
}

// Définition des autres fonctions.
EnsCoord voisines(Coord const c)
{
  EnsCoord ens;

  int lig = c.getLig();
  int col = c.getCol();

  int imin = std::max(lig - 1, 0);
  int imax = std::min(lig + 1, TAILLEGRILLE - 1);

  int jmin = std::max(col - 1, 0);
  int jmax = std::min(col + 1, TAILLEGRILLE - 1);

  for (int i = imin; i <= imax; i++)
    for (int j = jmin; j <= jmax; j++)
      if (i != lig or j != col)
        ens.ajoute(Coord(i, j));

  return ens;
}

TEST_CASE("Test de la fonction voisines")
{
  std::vector<Coord> v1 = std::vector<Coord>({Coord(1, 0), Coord(1, 1), Coord(1, 2),
                                              Coord(2, 2), Coord(3, 2), Coord(3, 1),
                                              Coord(3, 0), Coord(2, 0)});

  std::vector<Coord> v2 = std::vector<Coord>({Coord(2, 3), Coord(2, 4), Coord(3, 3),
                                              Coord(4, 3), Coord(4, 4)});

  std::vector<Coord> v3 = std::vector<Coord>({Coord(0, 1), Coord(1, 0), Coord(1, 1)});

  CHECK(voisines(Coord(2, 1)) == EnsCoord(v1));
  CHECK(voisines(Coord(3, 4)) == EnsCoord(v2));
  CHECK(voisines(Coord(0, 0)) == EnsCoord(v3));
}

Coord choiXHasard(EnsCoord const &ens)
{
  return ens.ieme(std::rand() % (ens.taille() + 1));
}

TEST_CASE("Test de la fonction choixHasard")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 0), Coord(1, 1), Coord(1, 2)}));
  CHECK(ens.contient(choiXHasard(ens)));
}