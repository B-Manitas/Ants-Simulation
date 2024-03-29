#ifndef Grid_HPP
#define Grid_HPP

#include "Ant.hpp"
#include "Coord.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"

#include <iostream>
#include <vector>
#include <sstream>

class Grid
{
private:
  std::vector<std::vector<Place>> m_grid;

public:
  Grid() : m_grid(std::vector<std::vector<Place>>()){};
  Grid(int const size);

  /**Return the size of the grid.*/
  size_t getSize() const { return m_grid.size(); };
  /**
   * Get a place in the grid.
   * @param coord The coordinate of the place.
   * @return Return the place at coordinate specified.
  */
  Place getPlace(Coord const &coord) const;
  /**Return a random place in the grid.*/
  Place getRandPlace() const { return getPlace(Coord(std::rand() % getSize(), std::rand() % getSize())); };
  /**Return a random empty place in the grid.*/
  Place getRandEmptyPlace() const;
  /**
   * An empty random neighbour.
   * @param coord The coordinate of the place.
   * @return Return an empty place. If no empty place is find, return a ghost place. 
  */
  Place getRandEmptyNeighbour(Coord const &coord) const;
  /**
   * Set the place in the grid.
   * @param coord The place to put in the grid.
  */
  void setPlace(Place const &place) { m_grid[place.getCoord().getY()][place.getCoord().getX()] = place; };
  /**Linearization of the pheromones of the nest in the grid.*/
  void linearizePheroAntNest();
  /**
   * Decrease the sugar pheromones, for all places on the grid.
   * @param quantity The quantity of sugar. By default, the quantity is 5.
  */
  void decreasePheroSugar(int const quantity = 5);

  friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

/**
 * Set sugar in the grid.
 * @param grid The grid.
 * @param set_sugars The set containing the coordinates of the sugars. 
*/
void setSugars(Grid &grid, EnsCoord &set_sugars);
/**
 * Set water in the grid.
 * @param grid The grid.
 * @param set_water The set containing the coordinates of the sugars. 
*/
void set_water(Grid &grid, EnsCoord &set_water);
/**
 * Set ants in the grid.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param set_ants The set containing the coordinates of the ants. 
*/
void setAnts(Grid &grid, GridAnts &ants, EnsCoord &set_ants);
/**
 * Set ants nests in the grid.
 * @param grid The grid.
 * @param set_nests The set containing the coordinates of the nests. 
*/
void setAntsNests(Grid &grid, EnsCoord &set_nests);
/**
 * Initialize the sets with random coordinates.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param set_nests The set of coordinates of the ants nests.
 * @param nb_sugar The number of sugars. By default, the number is 5.
 * @param nb_ants The numer of ants. By default, the number is 10.
*/
void initializeRandomPlaces(Grid &grid, GridAnts &ants, EnsCoord &set_nests, unsigned int nb_sugar = 5, unsigned int nb_ants = 10, bool add_sugar = true);
/**
 * Initialize the grid with the sets of sugars, ants and nests.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param set_sugars The set containing the coordinates of the sugars.
 * @param set_ants The set containing the coordinates of the ants.
 * @param set_nests The set containing the coordinates of the nests.
*/
void initializeGrid(Grid &grid, GridAnts &ants, EnsCoord &set_water, EnsCoord &set_sugars, EnsCoord &set_ants, EnsCoord &set_nests);

/**
 * Update an ants.
 * @param ant The ant to update.
 * @param grid The grid.
*/
void updateAnt(Ant &ant, Grid &grid);
/**
 * Update a set of ants.
 * @param grid The grid.
 * @param ants The sets of ants.
*/
void updateSetAnts(Grid &grid, GridAnts &ants);
/**
 * Make an evolution if is possible.
 * @param grid The grid.
 * @param ants The sets of ants.
 * @param set_nests The set of nests.
 * @param is_enough_food True if there is enough food, otherwise false.
 * @param nb_new_ant The number of sugar per nest required to create a new ant. By default, it's 5.
 * @param nb_new_nest The number of ant per nest required to create a new nest. By default, it's 50.
 * @param new_sugar The number of ant per nest required to create a new nest. By default, it's 71.
 * @return Return true if there is ants. Otherwise, return false.
*/
bool evolution(Grid &grid, GridAnts &ants, EnsCoord &set_nests, bool &is_enough_food, int &reserve_sugar,
               int iteration, int nb_new_ant = 5, int nb_new_nest = 50, int new_sugar = 71);
/**
 * Test if the simulation is consistent with the grid and the position of the ants.
 * Throw an exception if an inconsistency is found.
 * @param grid The grid containing all the simulation squares.
 * @param ants The grid containing all the ants in the simulation.
*/
void consistencyTest(Grid &grid, GridAnts &ants, std::string title);
/**
 * Create a 2D vector with the state of each square.
 * @param grid The grid containing all the simulation squares.
 * @param ants The grid containing all the ants in the simulation.
 * @return Returns a 2D vector of integer corresponding to the state of the square.
*/
std::vector<std::vector<int>> getGridState(Grid &grid, GridAnts &ants);

#endif
