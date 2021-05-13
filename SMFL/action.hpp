#ifndef ACTION_HPP
#define ACTION_HPP
#include "fourmi.hpp"
#include "place.hpp"

// Défition des conditions.
bool condition1(Fourmi f, Place p2);
bool condition2(Fourmi f, Place p2);
bool condition3(Fourmi f, Place p2);
bool condition4(Fourmi f, Place p1, Place p2);
bool condition5(Fourmi f, Place p1, Place p2);
bool condition6(Fourmi f, Place p1, Place p2);
bool condition7(Fourmi f, Place p2);
bool condtion_n(int n, Fourmi f, Place p1, Place p2);

// Défition des actions.
void action2(Fourmi &f, Place &p1);
void action3(Fourmi &f);
void action4(Fourmi &f, Place &p1, Place &p2);
void action567(Fourmi &f, Place &p1, Place &p2);
void action_n(int n, Fourmi &f, Place &p1, Place &p2);

#endif