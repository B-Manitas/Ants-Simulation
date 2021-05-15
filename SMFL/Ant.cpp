#include "Ant.hpp"
#include "Coord.hpp"

#include <sstream>

Ant::Ant(Coord c, int index) : m_coord{c}, m_id{index}, m_isCarryingSugar{false}
{
  if (m_id < 0 and m_id != -1)
  {
    std::ostringstream msg_error;
    msg_error << "Error Constructor Ant : The index of the ant must be positif or equal to -1, if it's a ghost ant."
              << "\nThe index passed in parameter is equal to "
              << index
              << ".";
    throw std::invalid_argument(msg_error.str());
  }
}

std::ostream &operator<<(std::ostream &out, const Ant &ant)
{
  out << "{ Coord: " << ant.getCoord()
      << ", Id: " << ant.getIndex()
      << ", Sugar: " << ((ant.lookForSugar() == true) ? "Yes" : "No")
      << " }";

  return out;
}
