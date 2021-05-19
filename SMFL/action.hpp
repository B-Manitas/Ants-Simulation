#ifndef ACTION_HPP
#define ACTION_HPP

#include "Ant.hpp"
#include "Place.hpp"

/**
 * Return true if the ant is alive and the place p2 contains an ant, otherwise false. 
 * @param ant The ant.
 * @param p2 The place p2.
*/
bool condition0(Ant ant, Place p2);
/**
 * Return true if walk on the water place, otherwise false. 
 * @param ant The ant.
 * @param p1 The place p1.
*/
bool condition1(Place p1);
/**
 * Returns true if the ant is looking for sugar and location p2 contains sugar, otherwise false. 
 * @param ant The ant.
 * @param p2 The place p2.
*/
bool condition2(Ant ant, Place p2);
/**
 * Returns true if the ant is looking for sugar and place p2 contains sugar, otherwise false.
 * @param ant The ant.
 * @param p2 The place p2.
*/
bool condition3(Ant ant, Place p2);
/**
 * Return true if the ant come back to the nest, the place p2 is empty and 
 * the place p2 is closer to the nest than p1, otherwise false. 
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
bool condition4(Ant ant, Place p1, Place p2);
/**
 * Return true, if the ant is looking for sugar, the place p1 and p2 are on sugar trail, 
 * the place p2 is empty and the place p1 is closer to the nest than p2. Otherwise, return false. 
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
bool condition5(Ant ant, Place p1, Place p2);
/**
 * Return true if the ant is looking for sugar, the place p2 is on sugar trail and is empty.
 * Otherwise, return false. 
 * @param ant The ant.
 * @param p2 The place p2.
*/
bool condition6(Ant ant, Place p2);
/**
 * Return true, if the ant is looking for sugar and the place p2 is empty.
 * Otherwise, return false. 
 * @param ant The ant.
 * @param p2 The place p2.
*/
bool condition7(Ant ant, Place p2);
/**
 * Return nth condition. 
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
bool condtionNth(int n, Ant ant, Place p1, Place p2);

/**
 * The ant carries a sugar and put down sugar pheromone on the place p1.
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
void action2(Ant &ant, Place &p1, Place &p2);
/**
 * the ant brings sugar to the nest.
 * @param ant The ant.
 * @param p2 The place p2.
*/
void action3(Ant &f, Place &p2);
/**
 * The ant moves to location p2. And drop the sugar pheromone in place p2.
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
void action4(Ant &f, Place &p1, Place &p2);
/**
 * The ant moves from the place p1 to p2.
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
void actionMove(Ant &ant, Place &p1, Place &p2);
/**
 * Do nth action. 
 * @param ant The ant.
 * @param p1 The place p1.
 * @param p2 The place p2.
*/
void actionNth(int n, Ant &ant, Place &p1, Place &p2);

#endif