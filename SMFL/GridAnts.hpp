#ifndef GRID_ANTS_HPP
#define GRID_ANTS_HPP

#include "Ant.hpp"
#include "Coord.hpp"

#include <vector>

struct GridAnts
{
  std::vector<Ant> m_grid;
  int m_index;

  GridAnts() : m_grid{std::vector<Ant>()}, m_index{-1} {};
  /**
   * Get the position of the ant in the grid.
   * @param index The index of the ant.
   * @return Return the position in the grid of the ant.
  */
  int position(int const index) const;

  /**Get the size of the grid.*/
  size_t getSize() const { return m_grid.size(); };
  /**
   * Get the ant in the grid.
   * @param coord The coordinate.
   * @return Return the ant, to the coordinate passed in the parameter.
  */
  Ant getAnt(Coord const &coord) const;
  /**Return a random index not used an ants.*/
  int getNewIndex();
  /**
   * Set the ant in the grid.
   * @param coord The ant.
  */
  void setAnt(Ant const &ant);
  /**
   * Returns if the ant is in the grid.
   * @param id The index of the ant.
   * @return Returns true if the ant is in the grid, otherwise false.
   */
  bool isContainingAnts(int const index) const;
  /**
   * Returns if the ant is in the grid.
   * @param coord The coordinate of the ant.
   * @return Returns true if the ant is in the grid, otherwise false.
   */
  bool isContainingAntsCoord(Coord const &coord) const;

  /**
   * Revove the ant of the grid.
   * @param ant The ant to remove.
  */
  void remove(Ant const &ant);
  friend std::ostream &operator<<(std::ostream &out, GridAnts const &ants);
};

#endif