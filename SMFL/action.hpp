#ifndef ACTION_HPP
#define ACTION_HPP
#include "fourmi.hpp"
#include "place.hpp"

struct Action
{
public:
  // Défition des conditions.
  bool condition1(Fourmi f, Place p2) const { return p2.contientFourmi(); };
  bool condition2(Fourmi f, Place p2) const { return f.chercheSucre() && p2.contientSucre(); };
  bool condition3(Fourmi f, Place p2) const { return not f.chercheSucre() && p2.contientNid(); };
  bool condition4(Fourmi f, Place p1, Place p2) const { return not f.chercheSucre() && p2.estVide() && estPlusProcheNid(p2, p1); };
  bool condition5(Fourmi f, Place p1, Place p2) const { return f.chercheSucre() && p1.estSurUnePiste() && p2.estVide() && not estPlusProcheNid(p2, p1) && p2.estSurUnePiste(); };
  bool condition6(Fourmi f, Place p1, Place p2) const { return f.chercheSucre() && p2.estSurUnePiste() && p2.estVide(); };
  bool condition7(Fourmi f, Place p2) const { return f.chercheSucre() && p2.estVide(); };
  bool condtion_n(int n, Fourmi f, Place p1, Place p2) const;

  // Défition des actions.
  void action2(Fourmi &f, Place &p1);
  void action3(Fourmi &f);
  void action4(Fourmi &f, Place &p1, Place &p2);
  void action7(Fourmi &f, Place &p1, Place &p2);
  void action_n(int n, Fourmi &f, Place &p1, Place &p2);
};

#endif