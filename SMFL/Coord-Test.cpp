#include "Coord.hpp"
#include "Doctest.h"

#include <sstream>
#include <vector>

TEST_SUITE_BEGIN("Test de la classe Coord.");

TEST_CASE("Test du constructeur Coord")
{
  CHECK_THROWS_AS(Coord(-5, -5), std::invalid_argument);
  CHECK_THROWS_AS(Coord(-5, GRID_SIZE), std::invalid_argument);
  CHECK_THROWS_AS(Coord(GRID_SIZE + 5, GRID_SIZE + 5), std::invalid_argument);
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

  CHECK(EnsCoord(v).isContained(Coord(1, 2)));
  CHECK(EnsCoord(v).isContained(Coord(3, 4)));
  CHECK_FALSE(EnsCoord(v).isContained(Coord(5, 2)));
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
  ens.add(Coord(1, 2));
  CHECK(ens.isContained(Coord(1, 2)));

  ens.add(Coord(3, 2));
  CHECK(ens.isContained(Coord(3, 2)));
}

TEST_CASE("Test de la méthode supprime.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)}));

  ens.remove(Coord(1, 2));
  CHECK(not ens.isContained(Coord(1, 2)));

  ens.remove(Coord(3, 2));
  CHECK(not ens.isContained(Coord(3, 2)));
  CHECK_THROWS_AS(ens.remove(Coord(3, 2)), std::invalid_argument);
}

TEST_CASE("Test de la méthode estVide.")
{
  CHECK(EnsCoord(std::vector<Coord>()).isEmpty());
  CHECK(not EnsCoord(std::vector<Coord>({Coord(1, 2)})).isEmpty());
}

TEST_CASE("Test de la méthode taille.")
{
  CHECK(EnsCoord(std::vector<Coord>()).getSize() == 0);
  CHECK(EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)})).getSize() == 2);
}

TEST_CASE("Test de la méthode ieme.")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 2), Coord(3, 2)}));
  CHECK(ens.getNth(0) == Coord(1, 2));
  CHECK(ens.getNth(1) == Coord(3, 2));
  CHECK_THROWS_AS(ens.getNth(2), std::invalid_argument);
}

TEST_CASE("Test de la fonction voisines")
{
  if (GRID_SIZE == 5)
  {
    std::vector<Coord> v1 = std::vector<Coord>({Coord(1, 0), Coord(1, 1), Coord(1, 2),
                                                Coord(2, 2), Coord(3, 2), Coord(3, 1),
                                                Coord(3, 0), Coord(2, 0)});

    std::vector<Coord> v2 = std::vector<Coord>({Coord(2, 3), Coord(2, 4), Coord(3, 3),
                                                Coord(4, 3), Coord(4, 4)});

    std::vector<Coord> v3 = std::vector<Coord>({Coord(0, 1), Coord(1, 0), Coord(1, 1)});

    CHECK(neighbors(Coord(2, 1)) == EnsCoord(v1));
    CHECK(neighbors(Coord(3, 4)) == EnsCoord(v2));
    CHECK(neighbors(Coord(0, 0)) == EnsCoord(v3));
  }
}

TEST_CASE("Test de la fonction getRandNth")
{
  EnsCoord ens = EnsCoord(std::vector<Coord>({Coord(1, 0), Coord(1, 1), Coord(1, 2)}));
  CHECK(ens.isContained(ens.getRandNth()));
}
TEST_SUITE_END();