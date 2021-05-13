#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "doctest.h"
#include "coord.hpp"
#include <algorithm>

// Définition des méthodes de la classe Coord.
Coord::Coord(int x, int y) : m_x{x}, m_y{y}
{
  if (y < 0 or x < 0 or y > TAILLEGRILLE or x > TAILLEGRILLE)
    throw std::invalid_argument("La coordonnées est en dehors de la grille.");
}

int Coord::getY() const
{
  return m_y;
}

int Coord::getX() const
{
  return m_x;
}

Coord Coord::getCoord() const
{
  return Coord({m_x, m_y});
}

std::ostream &operator<<(std::ostream &out, const Coord &coord)
{
  return out << "(" << coord.getX() << ", " << coord.getY() << ")";
}

bool Coord::operator==(const Coord &coord) const
{
  return m_y == coord.getY() && m_x == coord.getX();
}

bool Coord::operator!=(const Coord &coord) const
{
  return !(*this == coord);
}

TEST_SUITE_BEGIN("Test de la classe Coord.");

TEST_CASE("Test du constructeur Coord")
{
  CHECK_THROWS_AS(Coord(-5, -5), std::invalid_argument);
  CHECK_THROWS_AS(Coord(-5, TAILLEGRILLE), std::invalid_argument);
  CHECK_THROWS_AS(Coord(TAILLEGRILLE + 5, TAILLEGRILLE + 5), std::invalid_argument);
}

TEST_CASE("Test du getter.")
{
  Coord A = Coord{2, 3};
  CHECK(A.getCoord() == Coord{2, 3});
  CHECK(A.getX() == 2);
  CHECK(A.getY() == 3);
}

TEST_CASE("Test de l'operateur <<")
{
  std::ostringstream stream;
  stream << Coord(2, 3);
  CHECK(stream.str() == "(2, 3)");
}

TEST_CASE("Test de l'operateur ==")
{
  CHECK(Coord{2, 3} == Coord{2, 3});
  CHECK_FALSE(Coord{2, 3} == Coord{3, 2});
}

TEST_CASE("Test de l'operateur !=")
{
  CHECK(Coord{2, 3} != Coord{3, 2});
  CHECK_FALSE(Coord{2, 3} != Coord{2, 3});
}

TEST_SUITE_END();

// Définition des méthodes de la classe EnsCoord.

std::ostream &operator<<(std::ostream &out, const EnsCoord &e)
{
  out << "{";
  for (int i = 0; i < e.taille(); i++)
  {
    out << e.ieme(i);
    if (i != e.taille() - 1)
      out << ", ";
  }
  out << "}";

  return out;
}

int EnsCoord::position(const Coord &c) const
{
  for (int i = 0; i < m_coords.size(); i++)
    if (m_coords[i] == c)
      return i;

  return -1;
}

bool EnsCoord::contient(const Coord &c) const
{
  return position(c) != -1;
}

bool EnsCoord::operator==(const EnsCoord &ens) const
{
  if (taille() != ens.taille())
    return false;

  for (int i = 0; i < taille(); i++)
    if (not contient(ens.ieme(i)))
      return false;

  return true;
}

bool EnsCoord::operator!=(const EnsCoord &ens) const
{
  return not(*this == ens);
}

void EnsCoord::ajoute(const Coord c)
{
  if (not contient(c))
    m_coords.push_back(c);
}

void EnsCoord::supprime(const Coord c)
{
  if (contient(c))
    m_coords.erase(m_coords.begin() + position(c));

  else
    throw std::runtime_error("La coordonnée n'a pas pu être supprimée.");
}

bool EnsCoord::estVide() const
{
  return m_coords.size() == 0;
}

int EnsCoord::taille() const
{
  return m_coords.size();
}

void EnsCoord::melange()
{
  std::random_shuffle(m_coords.begin(), m_coords.end());
}

Coord EnsCoord::ieme(const int &n) const
{

  if (n >= 0 and n < taille())
    return m_coords[n];

  else
    throw std::invalid_argument("Erreur EnsCoord: Le paramètre n est en dehors des bornes de l'ensemble.");
}

EnsCoord voisines(Coord const &c)
{
  EnsCoord ens;

  int y = c.getY();
  int x = c.getX();

  int ymin = std::max(y - 1, 0);
  int ymax = std::min(y + 1, TAILLEGRILLE - 1);

  int xmin = std::max(x - 1, 0);
  int xmax = std::min(x + 1, TAILLEGRILLE - 1);

  for (int l = ymin; l <= ymax; l++)
    for (int c = xmin; c <= xmax; c++)
      if (l != y or c != x)
        ens.ajoute(Coord(c, l));

  return ens;
}

Coord choixHasard(EnsCoord const &ens)
{
  return ens.ieme(std::rand() % ens.taille());
}

TEST_SUITE_BEGIN("Test de la fonction EnsCoord.");
TEST_CASE("Test de l'operateur << ")
{
  std::ostringstream stream;
  std::vector<Coord> v1 = std::vector<Coord>({Coord(1, 2)});
  std::vector<Coord> v2 = std::vector<Coord>({Coord(1, 2), Coord(3, 4)});

  stream << EnsCoord(v1);
  CHECK(stream.str() == "{(1, 2)}");

  stream.str("");
  stream << EnsCoord(v2);
  CHECK(stream.str() == "{(1, 2), (3, 4)}");
}

TEST_CASE("Test de la méthode contient.")
{
  std::vector<Coord> v = std::vector<Coord>({Coord(1, 2), Coord(3, 4)});

  CHECK(EnsCoord(v).contient(Coord(1, 2)));
  CHECK(EnsCoord(v).contient(Coord(3, 4)));
  CHECK_FALSE(EnsCoord(v).contient(Coord(5, 2)));
}

TEST_CASE("Test de l'opérateur == et !=.")
{
  EnsCoord ens1 = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 4)}));
  EnsCoord ens2 = EnsCoord(std::vector<Coord>({Coord(3, 4), Coord(1, 2)}));
  EnsCoord ens3 = EnsCoord(std::vector<Coord>({Coord(2, 3)}));
  CHECK(ens1 == ens1);
  CHECK(ens1 == ens2);
  CHECK(ens1 != ens3);
}

TEST_CASE("Test de la méthode ajoute.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>());
  ens.ajoute(Coord(1, 2));
  CHECK(ens.contient(Coord(1, 2)));

  ens.ajoute(Coord(3, 2));
  CHECK(ens.contient(Coord(3, 2)));
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

TEST_CASE("Test de la méthode estVide.")
{
  CHECK(EnsCoord(std::vector<Coord>()).estVide());
  CHECK(not EnsCoord(std::vector<Coord>({Coord(1, 2)})).estVide());
}

TEST_CASE("Test de la méthode taille.")
{
  CHECK(EnsCoord(std::vector<Coord>()).taille() == 0);
  CHECK(EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)})).taille() == 2);
}

TEST_CASE("Test de la méthode ieme.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)}));
  CHECK(ens.ieme(0) == Coord(1, 2));
  CHECK(ens.ieme(1) == Coord(3, 2));
  CHECK_THROWS_AS(ens.ieme(2), std::invalid_argument);
}

TEST_CASE("Test de la fonction voisines")
{
  if (TAILLEGRILLE == 5)
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
}

TEST_CASE("Test de la fonction choixHasard")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 0), Coord(1, 1), Coord(1, 2)}));
  CHECK(ens.contient(choixHasard(ens)));
}
TEST_SUITE_END();
