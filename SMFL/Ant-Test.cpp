#include "Ant.hpp"
#include "Coord.hpp"
#include "Doctest.h"

#include <sstream>

TEST_SUITE_BEGIN("Ant class tests.");

TEST_CASE("Ant constructor.")
{
  CHECK_THROWS_AS(Ant(Coord(), -5), std::invalid_argument);
}

TEST_CASE("Ant getCoord.")
{
  CHECK(Ant(Coord(0, 0), 0).getCoord() == Coord(0, 0));
  CHECK(Ant(Coord(1, 2), 1).getCoord() == Coord(1, 2));
}

TEST_CASE("Ant getIndex.")
{
  CHECK(Ant(Coord(0, 0), 0).getIndex() == 0);
  CHECK(Ant(Coord(1, 2), 1).getIndex() == 1);
}

TEST_CASE("Ant lookForSugar.")
{
  Ant ant_1, ant_2;
  ant_1.carrySugar();

  CHECK_FALSE(ant_1.lookForSugar());
  CHECK(ant_2.lookForSugar());
}

TEST_CASE("Ant carrySugar.")
{
  Ant ant_1, ant_2;
  ant_2.carrySugar();

  CHECK(ant_1.lookForSugar());
  CHECK_FALSE(ant_2.lookForSugar());
}

TEST_CASE("Ant putSugar.")
{
  Ant ant_1, ant_2;
  ant_2.carrySugar();
  ant_1.carrySugar();
  ant_1.putSugar();

  CHECK(ant_1.lookForSugar());
  CHECK_FALSE(ant_2.lookForSugar());
}

TEST_CASE("Ant move.")
{
  Ant ant(Coord(0, 0), 0);
  ant.move(Coord(1, 1));

  CHECK(ant.getCoord() == Coord(1, 1));
}

TEST_CASE("Ant operator == and !=.")
{
  Ant ant = Ant(Coord(0, 0), 0);
  Ant an_2 = ant;
  Ant an_3 = Ant();

  CHECK(ant == ant);
  CHECK(ant == an_2);
  CHECK(ant != an_3);
}

TEST_CASE("Ant opertor <<.")
{
  Ant ant = Ant(Coord(1, 2), 3);
  std::ostringstream stream;
  stream << ant;
  CHECK(stream.str() == "{ Coord: (1, 2), Id: 3, Sugar: No }");
}
TEST_SUITE_END();
