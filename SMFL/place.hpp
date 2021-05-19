#ifndef PLACE_H
#define PLACE_H

#include "Ant.hpp"
#include "Coord.hpp"

#include <iostream>

class Place
{
private:
  Coord m_coord;
  int m_idAnt, m_food_reserve;
  int m_pieceSugar, m_pieceAntNest;
  int m_pheroSugar;
  double m_pheroAntNest;
  bool m_is_ghost, m_is_water;

public:
  Place() : m_coord{Coord()}, m_idAnt{-1},
            m_food_reserve{0},
            m_pieceSugar{0}, m_pieceAntNest{0},
            m_pheroSugar{0}, m_pheroAntNest{0},
            m_is_ghost{false}, m_is_water{false} {};

  Place(Coord coord) : m_coord{coord}, m_idAnt{-1},
                       m_food_reserve{0},
                       m_pieceSugar{0}, m_pieceAntNest{0},
                       m_pheroSugar{0}, m_pheroAntNest{0},
                       m_is_ghost{false}, m_is_water{false} {};

  Place(bool is_ghost) : m_coord{Coord()}, m_idAnt{-1},
                         m_food_reserve{0},
                         m_pieceSugar{0}, m_pieceAntNest{0},
                         m_pheroSugar{0}, m_pheroAntNest{0},
                         m_is_ghost{is_ghost}, m_is_water{false} {};

  /**Get the coordinate of the place.*/
  Coord getCoord() const { return m_coord; };
  /**Get the ant index of the place.*/
  int getIdAnt() const { return m_idAnt; };
  /**Get the food reserve of the place.*/
  int getFoodReserve() const { return m_food_reserve; }
  /**Get the value of sugar pheromones at the place.*/
  double getPheroSugar() const { return m_pheroSugar; };
  /**Get the value of ant nest pheromones at the place.*/
  double getPheroAntNest() const { return m_pheroAntNest; };
  /**Get the number of sugar cubes from the place.*/
  int getPieceSugar() const { return m_pieceSugar; };
  /**Get the number of ant nest pieces from the place.*/
  int getPieceAntNest() const { return m_pieceAntNest; };

  /**Return if the place is a ghost place.*/
  bool isGhost() const { return m_is_ghost; };
  /**Returns true if the place a food, otherwise false.*/
  bool isContainingFood() const { return m_food_reserve > 0; };
  /**Returns true if the place contains sugar, otherwise false.*/
  bool isContainingSugar() const { return m_pieceSugar > 0; };
  /**Return if the place is a ghost place.*/
  bool isContainingWater() const { return m_is_water; };
  /**Returns true if the place contains an ant nest, otherwise false.*/
  bool isContainingAntNest() const { return m_pieceAntNest > 0; };
  /**Returns true if the place contains an ant, otherwise false.*/
  bool isContainingAnt() const { return m_idAnt != -1; };
  /**Returns true if the place contains a sugar pheromone, otherwise false.*/
  bool isContainingPheroSugar() const { return m_pheroSugar > 0; }
  /**Returns true if the place is empty, otherwise false.*/
  bool isEmpty() const { return not isContainingSugar() && not isContainingAntNest() && not isContainingAnt(); }
  /**Returns true if the place is on sugar trail, otherwise false.*/
  bool isOnSugarTrail() const { return m_pheroSugar > 0; };

  /**Store a food if the place is a nested.*/
  bool storeFood(int quantity = 1);
  /**Eat a quantity of food.*/
  void consumeFood(int quantity = 1) { m_food_reserve = m_food_reserve - quantity; };
  /**
   * Put sugar on the place.
   * @param quantity The quantity of sugar. By default, the quantity is 5.
   * @return Return true if the sugar has been putted, otherwise false.
  */
  bool putSugar(int quantity = 5);
  /**
   * Put ant on the place.
   * @param ant The ant.
   * @return Return true if the ant has been putted, otherwise false.
  */
  bool putAnt(Ant ant);
  /**
   * Put an ant nest on the place.
   * @return Return true if the ant nest has been putted, otherwise false.
  */
  bool putAntNest();
  /**
   * Put sugar pheromone on the place.
   * @param quantity The quantity of sugar pheromone. By default, the quantity is 255.
   * @return Return true if the sugar pheromone has been putted, otherwise false.
  */
  void putPheroSugar(double quantity = 255);
  /**
   * Put pheromones from the ant nest on the place.
   * @param quantity The quantity of sugar..
   * @return Return true if the pheromones from the ant nest has been putted, otherwise false.
  */
  void putPheroAntNest(double quantity);
  /**
   * Put pheromones on the place.
  */
  bool putWater();

  /**
   * Remove sugar on the place.
   * @param quantity The quantity of sugar. By default, the quantity is 5.
  */
  void removeSugar(int quantity = 5) { m_pieceSugar = std::max(m_pieceSugar - quantity, 0); };
  /**Remove the ant on the place.*/
  void removeAnt() { m_idAnt = -1; };
  /** Remove sugar pheromone on the place.*/
  void removePheroSugar() { m_pheroSugar = 0; };
  /** Remove water on the place.*/
  void removeWater() { m_is_water = false; };
  /**
   * Decrease the quantity of sugar pheromone on the place.
   * @param quantity The quantity of sugar. By default, the quantity is 5.
  */
  void decreasePheroSugar(int quantity = 5) { m_pheroSugar = std::max(m_pheroSugar - quantity, 0); };

  bool operator==(const Place &p) const;
  bool operator!=(const Place &p) const { return !(*this == p); };
  friend std::ostream &operator<<(std::ostream &out, const Place &place);
};

/**
 * Returns if place p1 is closer to the ant nest than place p2.
 * @param p1 The first place.
 * @param p2 The second place.
 * @return Returns true if place p1 is closer to the ant nest than place p2, otherwise false.
 */
bool isCloserToNest(Place p1, Place p2);

/**
 * Move the ant from place p1 to place p2.
 * @param ant The ant to move.
 * @param p1 The current place of the ant.
 * @param p2 The place where the ant will be moved.
 */
void moveAnt(Ant &ant, Place &p1, Place &p2);

#endif