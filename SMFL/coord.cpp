#include "Coord.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Coord::Coord(int x, int y) : m_x{x}, m_y{y}
{
  if (y < 0 or x < 0 or y > GRID_SIZE or x > GRID_SIZE)
  {
    std::ostringstream msg_error;
    msg_error << "Error Constructor Coord : The coordinate ("
              << x
              << ", "
              << y
              << ") is outside the limits of the grid.";
    throw std::invalid_argument(msg_error.str());
  }
}

std::ostream &operator<<(std::ostream &out, const Coord &coord)
{
  return out << "(" << coord.getX() << ", " << coord.getY() << ")";
};

std::ostream &operator<<(std::ostream &out, const EnsCoord &set)
{
  out << "{";
  for (size_t i = 0; i < set.getSize(); i++)
  {
    out << set.getNth(i);
    if (i != set.getSize() - 1)
      out << ", ";
  }
  out << "}";

  return out;
}

int EnsCoord::position(const Coord &coord) const
{
  for (size_t i = 0; i < m_coords.size(); i++)
    if (m_coords[i] == coord)
      return i;

  return -1;
}

bool EnsCoord::operator==(const EnsCoord &set) const
{
  if (getSize() != set.getSize())
    return false;

  for (size_t i = 0; i < getSize(); i++)
    if (not isContained(set.getNth(i)))
      return false;

  return true;
}

void EnsCoord::add(const Coord coord)
{
  if (not isContained(coord))
    m_coords.push_back(coord);
}

void EnsCoord::remove(const Coord coord)
{
  if (isContained(coord))
    m_coords.erase(m_coords.begin() + position(coord));

  else
  {
    std::ostringstream msg_error;
    msg_error << "Errorr Remove EnsCoord: The coordinate "
              << coord
              << " has not been deleted.";

    throw std::invalid_argument(msg_error.str());
  }
}

Coord EnsCoord::getNth(const unsigned int &n) const
{

  if (n >= 0 and n < getSize())
    return m_coords[n];

  else
  {
    std::ostringstream msg_error;
    msg_error << "Error getNth EnsCoord : The parameter n= "
              << n
              << " is out of range.";
    throw std::invalid_argument(msg_error.str());
  }
}

EnsCoord neighbors(Coord const &coord)
{
  EnsCoord ens;

  int y = coord.getY();
  int x = coord.getX();

  int ymin = std::max(y - 1, 0);
  int ymax = std::min(y + 1, GRID_SIZE - 1);

  int xmin = std::max(x - 1, 0);
  int xmax = std::min(x + 1, GRID_SIZE - 1);

  for (int r = ymin; r <= ymax; r++)
    for (int c = xmin; c <= xmax; c++)
      if (r != y or c != x)
        ens.add(Coord(c, r));

  return ens;
}
