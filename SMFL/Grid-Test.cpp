#include "Coord.hpp"
#include "Doctest.h"
#include "Grid.hpp"
#include "Place.hpp"

TEST_SUITE_BEGIN("Grid class tests.");
TEST_CASE("Grid getSize.")
{
  CHECK(Grid(0).getSize() == 0);
  CHECK(Grid(2).getSize() == 2);
}

TEST_CASE("Grid getPlace.")
{
  CHECK(Grid(1).getPlace(Coord(0, 0)) == Place(Coord(0, 0)));
  CHECK(Grid(5).getPlace(Coord(4, 4)) == Place(Coord(4, 4)));
  CHECK_THROWS_AS(Grid(5).getPlace(Coord(5, 4)), std::invalid_argument);
  CHECK_THROWS_AS(Grid(5).getPlace(Coord(4, 5)), std::invalid_argument);

  Grid grid = Grid(2);
  Coord coord = Coord(1, 0);
  Place place = grid.getPlace(coord);
  place.putSugar();
  grid.setPlace(place);

  CHECK(grid.getPlace(coord) == place);
}

TEST_CASE("Grid getRandEmptyPlace.")
{
  Grid grid = Grid(2);
  Place place_1 = grid.getPlace(Coord(0, 0));
  Place place_2 = grid.getPlace(Coord(0, 0));
  place_1.putAntNest();
  place_2.putAntNest();
  grid.setPlace(place_1);
  grid.setPlace(place_2);
  Place rand_empty_place = grid.getRandEmptyPlace();
  CHECK(rand_empty_place.isEmpty());
}

TEST_CASE("Grid getRandEmptyNeighbour.")
{
  Grid grid = Grid(2);
  Place place_1 = grid.getPlace(Coord(0, 0));
  Place place_2 = grid.getPlace(Coord(0, 0));
  place_1.putAntNest();
  place_2.putAntNest();
  grid.setPlace(place_1);
  grid.setPlace(place_2);
  Place rand_empty_place = grid.getRandEmptyNeighbour(Coord(0, 0));
  CHECK(rand_empty_place.isEmpty());
}

TEST_CASE("Grid setPlace.")
{
  Grid grid = Grid(2);
  Place place = grid.getPlace(Coord(0, 0));
  place.putAntNest();
  grid.setPlace(place);
  Place place_2 = grid.getPlace(Coord(place.getCoord()));
  CHECK(place == place_2);
}

TEST_CASE("Grid decreasePheroSugar.")
{
  Grid grid = Grid(2);
  Place place = grid.getPlace(Coord(0, 0));
  place.putPheroSugar(255);
  grid.setPlace(place);
  grid.decreasePheroSugar(1);
  Place place_2 = grid.getPlace(Coord(place.getCoord()));
  CHECK(place_2.getPheroSugar() == 254);
}

TEST_SUITE_END();