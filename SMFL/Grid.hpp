#ifndef GRILLE_HPP
#define GRILLE_HPP

#include "Coord.hpp"
#include "Place.hpp"
#include "Ant.hpp"
#include "GridAnts.hpp"
#include <vector>
#include <iostream>

struct Grille
{
  std::vector<std::vector<Place>> m_grid;

  Grille() : m_grid(std::vector<std::vector<Place>>()){};
  Grille(int const size);

  /**Return the size of the grid.*/
  int getSize() const { return m_grid.size(); };
  /**
   * Get a place in the grid.
   * @param coord The coordinate of the place.
   * @return Return the place at coordinate specified.
  */
  Place getPlace(Coord const &coord) const;
  /**
   * Set the place in the grid.
   * @param coord The place to put in the grid.
  */
  void setPlace(Place const &place) { m_grid[place.getCoord().getY()][place.getCoord().getX()] = place; };
  /**Return a random place in the grid.*/
  Place getRandPlace() const { return getPlace(Coord(std::rand() % getSize(), std::rand() % getSize())); };
  /**Linearization of the pheromones of the nest in the grid.*/
  void linearizePheroAntNest();
  /**
   * Decrease the sugar pheromones, for all places on the grid.
   * @param quantity The quantity of sugar. By default, the quantity is 5.
  */
  void decreasePheroSugar(int const quantity = 5);

  friend std::ostream &operator<<(std::ostream &out, const Grille &g);
};

/**
 * Set sugar in the grid.
 * @param grid The grid.
 * @param set_sugars The set containing the coordinates of the sugars. 
*/
void setSugars(Grille &grid, EnsCoord &set_sugars);
/**
 * Set ants in the grid.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param set_ants The set containing the coordinates of the ants. 
*/
void setAnts(Grille &grid, GrilleFourmis &ants, EnsCoord &set_ants);
/**
 * Set ants nests in the grid.
 * @param grid The grid.
 * @param set_nests The set containing the coordinates of the nests. 
*/
void setAntsNests(Grille &grid, EnsCoord &set_nests);
/**
 * Initialize the sets with random coordinates.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param nb_sugar The number of sugars. By default, the number is 5.
 * @param nb_ants The numer of ants. By default, the number is 10.
*/
void initializeRandomPlaces(Grille &grid, GrilleFourmis &ants, int nb_sugar = 5, int nb_ants = 10);
/**
 * Initialize the grid with the sets of sugars, ants and nests.
 * @param grid The grid.
 * @param ants The grid of ants.
 * @param set_sugars The set containing the coordinates of the sugars.
 * @param set_ants The set containing the coordinates of the ants.
 * @param set_nests The set containing the coordinates of the nests.
*/
void initializeGrid(Grille &grid, GrilleFourmis &ants, EnsCoord &set_sugars, EnsCoord &set_ants, EnsCoord &set_nests);

/**
 * Update an ants.
 * @param ant The ant to update.
 * @param grid The grid.
*/
void updateAnt(Fourmi &ant, Grille &grid);
/**
 * Update a set of ants.
 * @param grid The grid.
 * @param ants The sets of ants.
*/
void updateSetAnts(Grille &grid, GrilleFourmis &ants);

#endif
