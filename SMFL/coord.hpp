#ifndef COORD_H
#define COORD_H

#include <iostream>
#include <vector>
#include <algorithm>

const int GRID_SIZE = 50;

class Coord
{
private:
  int m_x;
  int m_y;

public:
  Coord(){};
  Coord(int x, int y);

  /**Get the coordinate.*/
  Coord getCoord() const { return Coord({m_x, m_y}); };
  /**Get the x-axis coordinate.*/
  int getX() const { return m_x; };
  /**Get the y-axis coordinate.*/
  int getY() const { return m_y; };

  bool operator==(const Coord &coord) const { return m_y == coord.getY() && m_x == coord.getX(); };
  bool operator!=(const Coord &coord) const { return !(*this == coord); };
  friend std::ostream &operator<<(std::ostream &out, const Coord &coord);
};

class EnsCoord
{
private:
  std::vector<Coord> m_coords;
  int position(const Coord &c) const;

public:
  EnsCoord(){};
  EnsCoord(std::vector<Coord> set) : m_coords{set} {};

  /**
   * Returns whether the coordinate is contained in the set.
   * @param coord The coordinate.
   * @return Returns true if the coordinate is contained in the set, otherwise false. 
  */
  bool isContained(const Coord &coord) const { return position(coord) != -1; };
  /**
   * Return if the set is empty.
   * @return Return true if the set is empty, otherwise false.
  */
  bool isEmpty() const { return m_coords.size() == 0; };
  /**
   * Add the coordinate in the set.
   * @param coord The coordinate to add to the set.
  */
  void add(const Coord coord);
  /**
   * Remove the coordinate in the set.
   * @param coord The coordinate to remove to the set.
  */
  void remove(const Coord coord);
  /**Shuffle the set.*/
  void shuffle() { std::random_shuffle(m_coords.begin(), m_coords.end()); };
  /**Return the size of the set.*/
  int getSize() const { return m_coords.size(); };
  /**
   * Return the nth coordinate of the set.
   * @param n The nth integer of the set.
   * @return Return a coordinate of the set.
  */
  Coord nth(const int &n) const;

  friend std::ostream &operator<<(std::ostream &out, const EnsCoord &tableau);
  bool operator==(const EnsCoord &set) const;
  bool operator!=(const EnsCoord &set) const { return not(*this == set); };
};

/**
 * The set of neighbors of a coordinate.
 * @param coord The coordinate.
 * @return Returns all the coordinates around the one given in parameter.
*/
EnsCoord neighbors(Coord const &coord);
/**
 * A random of the set.
 * @param ens The set.
 * @return Return a random coordinate in the set.
*/
Coord randNth(EnsCoord const &set);

#endif
