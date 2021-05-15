#include "Ant.hpp"
#include "Coord.hpp"
#include "Place.hpp"

#include <sstream>

bool Place::storeFood()
{
  if (isContainingAntNest())
  {
    m_food_reserve++;
    return true;
  }

  return false;
}

bool Place::putSugar(int quantity)
{
  if (not isContainingAnt())
  {
    m_pieceSugar += quantity;
    putPheroSugar(255);
    return true;
  }

  return false;
};

bool Place::putAnt(Ant ant)
{
  if (isContainingAnt() or isContainingAntNest() or (isContainingSugar() && ant.lookForSugar()))
    return false;

  m_idAnt = ant.getIndex();
  return true;
};

bool Place::putAntNest()
{
  if (not isContainingAnt())
  {
    m_pieceAntNest++;
    putPheroAntNest(1);
    return true;
  }

  return false;
};

void Place::putPheroSugar(double quantity)
{
  if (quantity > 0)
    m_pheroSugar += (quantity > 255) ? 255 : quantity;

  m_pheroSugar = std::min(m_pheroSugar, 255);
};

void Place::putPheroAntNest(double quantity)
{
  if (quantity > 0)
    m_pheroAntNest += (quantity > 1) ? 1 : quantity;

  m_pheroAntNest = std::min(m_pheroAntNest, 1.);
};

std::ostream &operator<<(std::ostream &out, const Place &place)
{
  out << "{ "
      << "Coord: " << place.getCoord() << ", "
      << "Id Ant: " << place.getIdAnt() << ", "
      << "Phero Ant: " << place.getPheroAntNest() << ", "
      << "Phero Sugar: " << place.getPheroSugar() << ", "
      << "Ant: " << (place.isContainingAntNest() ? "Oui" : "Non") << ", "
      << "Sugar: " << (place.isContainingSugar() ? "Oui" : "Non") << " }";

  return out;
}

bool Place::operator==(const Place &p) const
{
  return (m_coord == p.getCoord() and
          m_pheroSugar == p.getPheroSugar() and
          m_pheroAntNest == p.getPheroAntNest() and
          m_idAnt == p.getIdAnt() and
          isContainingSugar() == p.isContainingSugar() and
          isContainingAntNest() == p.isContainingAntNest());
}

bool isCloserToNest(Place p1, Place p2)
{
  return p1.getPheroAntNest() > p2.getPheroAntNest();
};

void moveAnt(Ant &ant, Place &p1, Place &p2)
{
  if (p2.putAnt(ant))
  {
    ant.move(p2.getCoord());
    p1.removeAnt();
  }
}
