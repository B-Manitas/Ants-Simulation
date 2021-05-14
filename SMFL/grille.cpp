#include "grille.hpp"
#include "action.hpp"
#include "place.hpp"
#include "coord.hpp"
#include "fourmi.hpp"
#include "grilleFourmis.hpp"
#include "doctest.h"
#include <vector>
#include <iostream>
#include <sstream>

Grille::Grille(int const size)
{
  for (int y = 0; y < size; y++)
  {
    m_grid.push_back(std::vector<Place>());

    for (int x = 0; x < size; x++)
      m_grid[y].push_back(Place(Coord(x, y)));
  }
}

Place Grille::getPlace(const Coord &coord) const
{
  int coord_x = coord.getX();
  int coord_y = coord.getY();

  if (coord_x >= 0 && coord_x < m_grid[0].size() && coord_y >= 0 && coord_y < m_grid.size())
    return m_grid[coord_y][coord_x];

  else
    throw std::runtime_error("La coordonnée est en dehors de la grille.");
}

void Grille::linearizePheroAntNest()
{
  bool is_stable = false;

  while (not is_stable)
  {
    is_stable = true;
    for (int y = 0; y < getSize(); y++)
      for (int x = 0; x < getSize(); x++)
      {
        Coord c = Coord(x, y);
        Place p = getPlace(c);

        if (p.getPheroAntNest() < 1)
        {
          EnsCoord coordVoisins = neighbors(c);
          double maxPhero = 0;

          for (int posVois = 0; posVois < coordVoisins.getSize(); posVois++)
          {
            Place v = getPlace(coordVoisins.nth(posVois));
            maxPhero = std::max(maxPhero, v.getPheroAntNest());
          }

          maxPhero -= 1. / GRID_SIZE;

          if (maxPhero > p.getPheroAntNest())
          {
            p.putPheroAntNest(maxPhero);
            setPlace(p);
            is_stable = false;
          }
        }
      }
  }
}

void Grille::decreasePheroSugar(int const quantity)
{
  for (int y = 0; y < getSize(); y++)
    for (int x = 0; x < getSize(); x++)
    {
      Coord c = Coord(x, y);
      Place p = getPlace(c);
      p.decreasePheroSugar(quantity);
      setPlace(p);
    }
}

std::ostream &operator<<(std::ostream &out, const Grille &grid)
{
  out << "{";
  for (int i = 0; i < grid.m_grid.size(); i++)
  {
    for (int j = 0; j < grid.m_grid.size(); j++)
      out << grid.m_grid[i][j];

    out << std::endl;
  }
  out << "}";
  return out;
}

void setSugars(Grille &grid, EnsCoord &set_sugars)
{
  for (size_t i = 0; i < set_sugars.getSize(); i++)
  {
    Place place = grid.getPlace(set_sugars.nth(i));
    place.putSugar();
    grid.setPlace(place);
  }
}

void setAnts(Grille &grid, GrilleFourmis &ants, EnsCoord &set_ants)
{
  for (size_t i = 0; i < set_ants.getSize(); i++)
  {
    Coord coord = set_ants.nth(i);
    Place place = grid.getPlace(coord);
    Fourmi ant = Fourmi(coord, i);
    ants.setAnt(ant);
    place.putAnt(ant);
    grid.setPlace(place);
  }
}

void setAntsNests(Grille &grid, EnsCoord &set_nests)
{
  for (size_t i = 0; i < set_nests.getSize(); i++)
  {
    Place place = grid.getPlace(set_nests.nth(i));
    place.putAntNest();
    grid.setPlace(place);
  }
}

void initializeRandomPlaces(Grille &grid, GrilleFourmis &ants, int nb_sugar, int nb_ants)
{
  // Test if the initialization values are consistent.
  if (nb_sugar + nb_ants + 4 > GRID_SIZE)
  {
    std::ostringstream msg_error;
    msg_error << "Error Grid Initialization : The simulation cannot be performed because the number "
              << "of sugar, ants and nests is larger than the size of the grid."
              << "\nNumber of places for sugars: "
              << nb_sugar
              << "\nNumber of places for ants: "
              << nb_ants
              << "\nNumber of places for ants nested : 4";

    throw std::invalid_argument(msg_error.str());
  }

  // The set containing all coordinates of already places taken.
  EnsCoord set_places_taken;

  // Create the random sugar set.
  EnsCoord set_sugars;
  while (set_sugars.getSize() < nb_sugar)
  {
    Coord rand_coord = grid.getRandPlace().getCoord();

    if (not set_places_taken.isContained(rand_coord))
    {
      set_places_taken.add(rand_coord);
      set_sugars.add(rand_coord);
    }
  }

  // Create the random ants set.
  // Get a random coord in the grid, to put a nest.
  EnsCoord set_nests;
  Coord rand_coord = grid.getRandPlace().getCoord();
  while (set_places_taken.isContained(rand_coord))
    rand_coord = grid.getRandPlace().getCoord();

  set_places_taken.add(rand_coord);
  set_nests.add(rand_coord);

  // Add 3 others nest near to the main ant nest.
  EnsCoord set_nests_neighbors = neighbors(rand_coord);
  set_nests_neighbors.shuffle();
  for (size_t i = 0; i < set_nests_neighbors.getSize(); i++)
  {
    Coord place_nth = set_nests_neighbors.nth(i);
    set_places_taken.add(place_nth);
    set_nests.add(place_nth);

    // A nest is composed of only 4 places.
    if (set_nests.getSize() >= 4)
      break;
  }

  // Create the random ants set.
  EnsCoord set_ants;
  while (set_ants.getSize() < nb_ants)
  {
    for (size_t i = 0; i < set_nests_neighbors.getSize(); i++)
    {
      Coord coord = set_nests_neighbors.nth(i);
      if (not set_places_taken.isContained(coord))
      {
        set_places_taken.add(coord);
        set_ants.add(coord);
      }

      // Change the set of neighbors because all places are already takes.
      // While the set of ants is not yet complete.
      if (set_ants.getSize() < nb_ants and i == set_nests_neighbors.getSize() - 1)
        set_nests_neighbors = neighbors(randNth(set_nests_neighbors));
    }
  }

  initializeGrid(grid, ants, set_sugars, set_ants, set_nests);
  grid.linearizePheroAntNest();
}

void initializeGrid(Grille &grid, GrilleFourmis &ants, EnsCoord &set_sugars, EnsCoord &set_ants, EnsCoord &set_nests)
{
  setSugars(grid, set_sugars);
  setAnts(grid, ants, set_ants);
  setAntsNests(grid, set_nests);
}

void updateAnt(Fourmi &ant, Grille &grid)
{
  Coord coord = ant.getCoord();
  Place p1 = grid.getPlace(coord);
  EnsCoord set_neighbors = neighbors(coord);

  set_neighbors.shuffle();

  for (int nb_rule = 2; nb_rule < 8; nb_rule++)
    for (size_t i = 0; i < set_neighbors.getSize(); i++)
    {
      Coord coord_neighbour = set_neighbors.nth(i);
      Place p2 = grid.getPlace(coord_neighbour);
      if (condtionNth(nb_rule, ant, p1, p2))
      {
        actionNth(nb_rule, ant, p1, p2);
        grid.setPlace(p1);
        grid.setPlace(p2);
        return;
      }
    }
}

void updateSetAnts(Grille &grid, GrilleFourmis &ants)
{
  for (size_t i = 0; i < ants.getSize(); i++)
    updateAnt(ants.m_grid[i], grid);
}

TEST_SUITE_BEGIN("Test de la classe Grille.");
TEST_CASE("Test de la fonction getPlace.")
{
  CHECK(Grille(1).getPlace(Coord(0, 0)) == Place(Coord(0, 0)));
  CHECK(Grille(5).getPlace(Coord(4, 4)) == Place(Coord(4, 4)));
  CHECK_THROWS_AS(Grille(5).getPlace(Coord(5, 4)), std::runtime_error);
  CHECK_THROWS_AS(Grille(5).getPlace(Coord(4, 5)), std::runtime_error);
}

TEST_CASE("Test de la fonction getPlace.")
{
  Grille g = Grille(2);
  Coord c = Coord(1, 0);
  Place p = g.getPlace(c);
  p.putSugar();
  g.setPlace(p);

  CHECK(g.getPlace(c) == p);
}
TEST_SUITE_END();
