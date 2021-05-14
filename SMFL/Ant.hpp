#ifndef Fourmi_HPP
#define Fourmi_HPP
#include "Coord.hpp"

class Fourmi
{
private:
  Coord m_coord;
  int m_id;
  bool m_isCarryingSugar;

public:
  Fourmi() : m_coord{Coord()}, m_id{-1}, m_isCarryingSugar{false} {};
  Fourmi(Coord c, int index);

  /**Return the coordinate of the ant.*/
  Coord getCoord() const { return m_coord; };
  /**Return the index of the ant.*/
  int getIndex() const { return m_id; };

  /**Return true if the ant is carrying sugar, otherwise false.*/
  bool lookForSugar() const { return not m_isCarryingSugar; };
  /**The ant carries a sugar.*/
  void carrySugar() { m_isCarryingSugar = true; }
  /**The ant puts down its sugar.*/
  void putSugar() { m_isCarryingSugar = false; }
  /**
   * The ant moves in the new coordinate.
   * @param coord The coordinate where the ant will move.
   */
  void move(Coord const &coord) { m_coord = coord; };

  bool operator==(const Fourmi &ant) const { return ant.getCoord() == m_coord && ant.getIndex() == m_id; };
  bool operator!=(const Fourmi &ant) const { return not(*this == ant); };
  friend std::ostream &operator<<(std::ostream &out, const Fourmi &ant);
};

#endif