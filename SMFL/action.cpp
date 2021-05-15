#include "Action.hpp"
#include "Ant.hpp"
#include "Place.hpp"

bool condition1(Ant ant, Place p2) { return p2.isContainingAnt(); };
bool condition2(Ant ant, Place p2) { return ant.lookForSugar() && p2.isContainingSugar(); };
bool condition3(Ant ant, Place p2) { return not ant.lookForSugar() && p2.isContainingAntNest(); };
bool condition4(Ant ant, Place p1, Place p2) { return not ant.lookForSugar() && p2.isEmpty() && isCloserToNest(p2, p1); };
bool condition5(Ant ant, Place p1, Place p2) { return ant.lookForSugar() && p1.isOnSugarTrail() && p2.isEmpty() && not isCloserToNest(p2, p1) && p2.isOnSugarTrail(); };
bool condition6(Ant ant, Place p1, Place p2) { return ant.lookForSugar() && p2.isOnSugarTrail() && p2.isEmpty(); };
bool condition7(Ant ant, Place p2) { return ant.lookForSugar() && p2.isEmpty(); };
bool condtionNth(int n, Ant ant, Place p1, Place p2)
{
  switch (n)
  {
  case 1:
    return false;
  case 2:
    return condition2(ant, p2);
  case 3:
    return condition3(ant, p2);
  case 4:
    return condition4(ant, p1, p2);
  case 5:
    return condition5(ant, p1, p2);
  case 6:
    return condition6(ant, p1, p2);
  case 7:
    return condition7(ant, p2);
  default:
    return false;
  }
}

void action2(Ant &ant, Place &p1, Place &p2)
{
  ant.carrySugar();
  p2.removeSugar(1);
  p1.putPheroSugar();
}

void action3(Ant &ant, Place &p2)
{
  ant.putSugar();
  p2.storeFood();
}

void action4(Ant &ant, Place &p1, Place &p2)
{
  moveAnt(ant, p1, p2);
  p2.putPheroSugar();
}

void actionMove(Ant &ant, Place &p1, Place &p2)
{
  if (p1 == p2)
    std::cout << p1 << std::endl;
  moveAnt(ant, p1, p2);
}

void actionNth(int n, Ant &ant, Place &p1, Place &p2)
{
  switch (n)
  {
  case 1:
    break;
  case 2:
    action2(ant, p1, p2);
    break;

  case 3:
    action3(ant, p2);
    break;

  case 4:
    action4(ant, p1, p2);
    break;

  case 5:
    actionMove(ant, p1, p2);
    break;

  case 6:
    actionMove(ant, p1, p2);
    break;

  case 7:
    actionMove(ant, p1, p2);
    break;

  default:
    break;
  }
}
