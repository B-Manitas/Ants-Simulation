#ifndef ACTION_HPP
#define ACTION_HPP
#include "fourmi.hpp"
#include "place.hpp"

struct Action
{
public:
  // Défition des conditions.
  bool condition1(Fourmi f, Place p1, Place p2) { return p2.contientFourmi(); };
  bool condition2(Fourmi f, Place p1, Place p2) { return not f.getContientSucre() && p2.contientSucre(); };
  bool condition3(Fourmi f, Place p1, Place p2) { return f.getContientSucre() && p2.contientNid(); };
  bool condition4(Fourmi f, Place p1, Place p2) { return f.getContientSucre() && p2.estVide() && estPlusProcheNid(p2, p1); };
  bool condition5(Fourmi f, Place p1, Place p2) { return f.getContientSucre() && p2.estSurUnePiste() && p2.estVide() && estPlusProcheNid(p1, p2) && p1.estSurUnePiste(); };
  bool condition6(Fourmi f, Place p1, Place p2) { return f.getContientSucre() && p2.estSurUnePiste() && p2.estVide(); };
  bool condition7(Fourmi f, Place p1, Place p2) { return f.getContientSucre() && p2.estVide(); };
  bool condtion_n(int n, Fourmi f, Place p1, Place p2);

  // Défition des actions.
  void action2(Fourmi f, Place p1, Place p2);
  void action3(Fourmi f, Place p1, Place p2);
  void action4(Fourmi f, Place p1, Place p2);
  void action7(Fourmi f, Place p1, Place p2);
  void action_n(int n, Fourmi f, Place p1, Place p2);
};

#endif