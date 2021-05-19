#include "Action.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "Grid.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"

#include <iostream>
#include <sstream>
#include <vector>

Grid::Grid(int const size)
{
  for (int y = 0; y < size; y++)
  {
    m_grid.push_back(std::vector<Place>());

    for (int x = 0; x < size; x++)
      m_grid[y].push_back(Place(Coord(x, y)));
  }
}

Place Grid::getPlace(const Coord &coord) const
{
  unsigned int coord_x = coord.getX();
  unsigned int coord_y = coord.getY();

  if (coord_x >= 0 && coord_x < m_grid[0].size() && coord_y >= 0 && coord_y < m_grid.size())
    return m_grid[coord_y][coord_x];

  else
  {
    std::ostringstream msg_error;
    msg_error << "Error Constructor Coord : The coordinate "
              << coord
              << " is outside the limits of the grid.";
    throw std::invalid_argument(msg_error.str());
  }
}

Place Grid::getRandEmptyPlace() const
{
  Place rand_place = getRandPlace();
  while (not rand_place.isEmpty())
    rand_place = getRandPlace();

  return rand_place;
}

Place Grid::getRandEmptyNeighbour(Coord const &coord) const
{
  EnsCoord set_neighbors = neighbors(coord);
  for (size_t i = 0; i < set_neighbors.getSize(); i++)
  {
    Place place = getPlace(set_neighbors.getNth(i));
    if (place.isEmpty())
      return place;
  }

  return Place(true);
}

void Grid::linearizePheroAntNest()
{
  bool is_stable = false;

  while (not is_stable)
  {
    is_stable = true;
    for (size_t y = 0; y < getSize(); y++)
      for (size_t x = 0; x < getSize(); x++)
      {
        Coord coord = Coord(x, y);
        Place place = getPlace(coord);

        if (place.getPheroAntNest() < 1)
        {
          EnsCoord coord_neighbour = neighbors(coord);
          coord_neighbour.shuffle();
          double max_phero_nest = 0;

          for (size_t i = 0; i < coord_neighbour.getSize(); i++)
          {
            Place place_neighbour = getPlace(coord_neighbour.getNth(i));
            max_phero_nest = std::max(max_phero_nest, place_neighbour.getPheroAntNest());
          }

          max_phero_nest = max_phero_nest - 1. / GRID_SIZE;

          if (max_phero_nest > place.getPheroAntNest())
          {
            place.putPheroAntNest(max_phero_nest);
            setPlace(place);
            is_stable = false;
          }
        }
      }
  }
}

void Grid::decreasePheroSugar(int const quantity)
{
  for (size_t y = 0; y < getSize(); y++)
    for (size_t x = 0; x < getSize(); x++)
    {
      Coord c = Coord(x, y);
      Place p = getPlace(c);
      p.decreasePheroSugar(quantity);
      setPlace(p);
    }
}

std::ostream &operator<<(std::ostream &out, const Grid &grid)
{
  out << "{";
  for (size_t y = 0; y < grid.getSize(); y++)
  {
    for (size_t x = 0; x < grid.getSize(); x++)
      out << grid.getPlace(Coord(x, y));

    out << std::endl;
  }
  out << "}";
  return out;
}

void setSugars(Grid &grid, EnsCoord &set_sugars)
{
  for (size_t i = 0; i < set_sugars.getSize(); i++)
  {
    Place place = grid.getPlace(set_sugars.getNth(i));
    place.putSugar();
    grid.setPlace(place);
  }
}

void setWater(Grid &grid, EnsCoord &set_water)
{
  for (size_t i = 0; i < set_water.getSize(); i++)
  {
    Place place = grid.getPlace(set_water.getNth(i));
    place.putWater();
    grid.setPlace(place);
  }
}

void setAnts(Grid &grid, GridAnts &ants, EnsCoord &set_ants)
{
  for (size_t i = 0; i < set_ants.getSize(); i++)
  {
    Coord coord = set_ants.getNth(i);
    Place place = grid.getPlace(coord);
    Ant ant = Ant(coord, ants.getNewIndex());
    ants.setAnt(ant);
    place.putAnt(ant);
    grid.setPlace(place);
  }
}

void setAntsNests(Grid &grid, EnsCoord &set_nests)
{
  for (size_t i = 0; i < set_nests.getSize(); i++)
  {
    Place place = grid.getPlace(set_nests.getNth(i));
    place.putAntNest();
    grid.setPlace(place);
  }
}

void initializeRandomPlaces(Grid &grid, GridAnts &ants, EnsCoord &set_nests,
                            unsigned int nb_sugar, unsigned int nb_ants,
                            bool add_sugar)
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
  while (set_sugars.getSize() < nb_sugar and add_sugar)
  {
    Coord rand_coord = grid.getRandPlace().getCoord();

    if (not set_places_taken.isContained(rand_coord))
    {
      set_places_taken.add(rand_coord);
      set_sugars.add(rand_coord);
    }
  }

  // Create the random water set.
  EnsCoord set_water;
  while (set_water.getSize() < 5)
  {
    Coord rand_coord = grid.getRandPlace().getCoord();

    if (not set_places_taken.isContained(rand_coord))
    {
      set_places_taken.add(rand_coord);
      set_water.add(rand_coord);
    }
  }

  // Create the random ants set.
  // Get a random coord in the grid, to put a nest.
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
    Coord place_nth = set_nests_neighbors.getNth(i);
    set_places_taken.add(place_nth);
    set_nests.add(place_nth);

    // A nest is composed of only 4 places.
    if (set_nests.getSize() >= 4)
      break;
  }

  // Create the random ants set.
  EnsCoord set_ants;
  while (set_ants.getSize() < nb_ants and add_sugar)
  {
    for (size_t i = 0; i < set_nests_neighbors.getSize(); i++)
    {
      Coord coord = set_nests_neighbors.getNth(i);
      if (not set_places_taken.isContained(coord))
      {
        set_places_taken.add(coord);
        set_ants.add(coord);
      }

      // Change the set of neighbors because all places are already takes.
      // While the set of ants is not yet complete.
      if (set_ants.getSize() < nb_ants and i == set_nests_neighbors.getSize() - 1)
        set_nests_neighbors = neighbors(set_nests_neighbors.getRandNth());
    }
  }

  initializeGrid(grid, ants, set_water, set_sugars, set_ants, set_nests);
  grid.linearizePheroAntNest();
}

void initializeGrid(Grid &grid, GridAnts &ants, EnsCoord &set_water, EnsCoord &set_sugars, EnsCoord &set_ants, EnsCoord &set_nests)
{
  setWater(grid, set_water);
  setSugars(grid, set_sugars);
  setAnts(grid, ants, set_ants);
  setAntsNests(grid, set_nests);
}

void updateAnt(Ant &ant, Grid &grid)
{
  Coord coord = ant.getCoord();
  Place p1 = grid.getPlace(coord);
  EnsCoord set_neighbors = neighbors(coord);

  set_neighbors.shuffle();

  for (int nb_rule = 1; nb_rule < 8; nb_rule++)
    for (size_t i = 0; i < set_neighbors.getSize(); i++)
    {
      Coord coord_neighbour = set_neighbors.getNth(i);
      Place p2 = grid.getPlace(coord_neighbour);
      if (condtionNth(nb_rule, ant, p1, p2))
      {
        if (nb_rule != 1)
        {
          actionNth(nb_rule, ant, p1, p2);
          grid.setPlace(p1);
          grid.setPlace(p2);
        }
        else
          ant.dies();

        return;
      }
    }
}

void updateSetAnts(Grid &grid, GridAnts &ants)
{
  for (size_t i = 0; i < ants.getSize(); i++)
  {
    Ant ant = ants.getNth(i);
    updateAnt(ant, grid);
    if (ant.isAlive())
      ants.setAnt(ant);

    else
    {
      Place place = grid.getPlace(ant.getCoord());
      place.removeAnt();
      grid.setPlace(place);
      ants.remove(ant);
    }
  }
}

bool evolution(Grid &grid, GridAnts &ants, EnsCoord &set_nests, bool &is_enough_food,
               int &reserve_sugar, int iteration, int nb_new_ant, int nb_new_nest, int new_sugar)
{
  // Add a sugar every X iterations.
  if (iteration > 0 and iteration % (50 + rand() % 50) == 0)
  {
    Place rand_sugar_place = grid.getRandEmptyPlace();

    if (rand_sugar_place.putSugar())
      grid.setPlace(rand_sugar_place);
  }

  // Remove sugar of the nest every X iterations.
  if (iteration > 0 and iteration % (200 + rand() % 150) == 0)
  {
    Coord nest_coord = set_nests.getRandNth();
    Place nest = grid.getPlace(nest_coord);
    nest.consumeFood(ants.getSize() / 3 + 1);
    grid.setPlace(nest);
  }

  // Calculate the quantity of food for all the nests.
  reserve_sugar = 0;

  for (size_t i = 0; i < set_nests.getSize(); i++)
  {
    Coord nest_coord = set_nests.getNth(i).getCoord();
    reserve_sugar += grid.getPlace(nest_coord).getFoodReserve();
  }

  // Test if a new nest can be created.
  if (reserve_sugar > 0 and ants.getSize() / set_nests.getSize() >= nb_new_nest * .1)
  {
    Place new_nest_place = grid.getRandEmptyNeighbour(set_nests.getRandNth());
    if (not new_nest_place.isGhost() and new_nest_place.putAntNest())
    {
      set_nests.add(new_nest_place.getCoord());
      grid.setPlace(new_nest_place);
    }
  }

  // Test if a new ant can be born.
  else if (ants.getSize() < LIMITS_ANT + 15 and reserve_sugar > 0 and reserve_sugar / ants.getSize() >= 1)
  {
    // Get a nest place containing food.
    Place nest_with_food = grid.getPlace(set_nests.getNth(0));
    for (size_t i = 1; i < set_nests.getSize(); i++)
    {
      if (not nest_with_food.isContainingFood())
        nest_with_food = grid.getPlace(set_nests.getNth(i));

      else
        break;
    }

    // Get a free place neighboor to nest where the ant will be birth.
    Place nest_nursery = grid.getPlace(set_nests.getNth(0));
    for (size_t i = 1; i < set_nests.getSize(); i++)
    {
      EnsCoord set_neighbors_nests = neighbors(set_nests.getNth(i));
      if (not nest_nursery.isEmpty())
        for (size_t i = 0; i < set_neighbors_nests.getSize(); i++)
          nest_nursery = grid.getPlace(set_neighbors_nests.getNth(i));

      else
        break;
    }

    // Create the ant.
    Ant new_ant = Ant(nest_nursery.getCoord(), ants.getNewIndex());
    if (nest_nursery.putAnt(new_ant))
    {
      nest_with_food.consumeFood();
      ants.setAnt(new_ant);
      grid.setPlace(nest_nursery);
    }
  }

  // Test if there is a famine.
  else if (ants.getSize() > LIMITS_ANT or reserve_sugar < ants.getSize() * 0.9)
  {
    is_enough_food = false;
    int nb_ants = ants.getSize() - 1;
    int nb_ants_die = ants.getSize() > 200 ? (nb_ants - nb_ants * .1) : 1;

    for (int i = nb_ants_die; i > 0; i--)
    {
      Ant ant = ants.getNth(ants.getSize() - i);
      Place place = grid.getPlace(ant.getCoord());
      place.removeAnt();
      grid.setPlace(place);
      ants.remove(ant);

      if (ants.getSize() < 1)
        return false;
    }
  }

  else
    is_enough_food = true;

  return true;
}

void consistencyTest(Grid &grid, GridAnts &ants, std::string title)
{
  std::ostringstream error_msg;

  // Take the first test. Test if the ant's index and place match.
  for (size_t i = 0; i < ants.getSize(); i++)
  {
    Ant ant = ants.getNth(i);
    Place place = grid.getPlace(ant.getCoord());

    if (place.getIdAnt() != ant.getIndex())
    {
      error_msg << title << std::endl;
      error_msg << "Inconsistent Simulation Error: The index of an ant does not correspond to its place." << std::endl
                << "Ant: " << ant << std::endl
                << "place: " << place << std::endl;

      throw std::runtime_error(error_msg.str());
    }
  }

  // Take the second test. Test if the place has a ghost ant.
  for (size_t y = 0; y < grid.getSize(); y++)
    for (size_t x = 0; x < grid.getSize(); x++)
    {
      Place place = grid.getPlace(Coord(x, y));

      if (place.isContainingAnt() && not ants.isContainingAnts(place.getIdAnt()))
      {
        error_msg << title << std::endl;
        error_msg << "Inconsistent Simulation Error: A place contains an ant that doesn't exist." << std::endl
                  << "place: " << place << std::endl
                  << "Ants: " << ants << std::endl;

        throw std::runtime_error(error_msg.str());
      }
    }
}

std::vector<std::vector<int>> getGridState(Grid &grid, GridAnts &ants)
{
  std::vector<std::vector<int>> color_grid(GRID_SIZE);

  for (size_t y = 0; y < grid.getSize(); y++)
    for (size_t x = 0; x < grid.getSize(); x++)
    {
      Coord square_coord = Coord(x, y);
      Place square = grid.getPlace(square_coord);
      Ant ant = Ant();

      if (ants.isContainingAntsCoord(square_coord))
        ant = ants.getAnt(square_coord);

      // Square containing an ant nest.
      if (square.isContainingAntNest())
        color_grid[y].push_back(int(place_state::nest));

      // Square containing some sugars.
      else if (square.isContainingSugar())
        color_grid[y].push_back(int(place_state::sugar));

      // Square containing some sugars.
      else if (square.isContainingWater())
        color_grid[y].push_back(int(place_state::water));

      // Square containing an ant.
      else if (square.isContainingAnt())
        color_grid[y].push_back(int(place_state::ant));

      // Square containing sugar pheromone.
      else if (square.isContainingPheroSugar())
        color_grid[y].push_back(int(place_state::phero_sugar));

      // The square is empty.
      else
        color_grid[y].push_back(int(place_state::empty));
    }

  return color_grid;
}