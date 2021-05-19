#include "Ant.hpp"
#include "Coord.hpp"
#include "GridAnts.hpp"

#include <sstream>

int GridAnts::position(int const index) const
{
  for (size_t i = 0; i < getSize(); i++)
    if (m_grid[i].getIndex() == index)
      return i;

  return -1;
}

Ant GridAnts::getAnt(Coord const &coord) const
{
  for (auto &ant : m_grid)
    if (ant.getCoord() == coord)
      return ant;

  std::ostringstream msg_error;
  msg_error << "Error getAnt GridAnts : There is no ant with the coordinate "
            << coord
            << " in the grid.";
  throw std::invalid_argument(msg_error.str());
}

Ant GridAnts::getNth(unsigned int const n)
{
  if (n >= 0 and n < getSize())
    return m_grid[n];
  else
  {
    std::ostringstream msg_error;
    msg_error << "Error getNth GridAnts : The parameter n= "
              << n
              << " is out of range.\nMax size: "
              << getSize();
    throw std::invalid_argument(msg_error.str());
  }
}

void GridAnts::setAnt(Ant const &ant)
{
  if (not isContainingAnts(ant.getIndex()))
    m_grid.push_back(ant);

  else
    m_grid[position(ant.getIndex())] = ant;
}

bool GridAnts::isContainingAnts(int const index) const
{
  for (auto &ant : m_grid)
    if (ant.getIndex() == index)
      return true;

  return false;
}

bool GridAnts::isContainingAntsCoord(Coord const &coord) const
{
  for (auto &ant : m_grid)
    if (ant.getCoord() == coord)
      return true;

  return false;
}

void GridAnts::remove(Ant const &ant)
{
  if (isContainingAntsCoord(ant.getCoord()))
    m_grid.erase(m_grid.begin() + position(ant.getIndex()));
}

int GridAnts::getNewIndex()
{
  m_index++;
  return m_index;
}

std::ostream &operator<<(std::ostream &out, GridAnts &ants)
{
  out << "{";
  for (size_t i = 0; i < ants.getSize(); i++)
  {
    out << ants.getNth(i);
    if (i != ants.getSize() - 1)
      out << ", ";
  }
  out << "}";

  return out;
}
