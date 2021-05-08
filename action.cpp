#include "action.hpp"
#include "fourmi.hpp"
#include "place.hpp"

bool Action::condtion_n(int n, Fourmi f, Place p1, Place p2)
{
  switch (n)
  {
  case 1:
    return condition1(f, p1, p2);
  case 2:
    return condition2(f, p1, p2);
  case 3:
    return condition3(f, p1, p2);
  case 4:
    return condition4(f, p1, p2);
  case 5:
    return condition5(f, p1, p2);
  case 6:
    return condition6(f, p1, p2);
  case 7:
    return condition7(f, p1, p2);

  default:
    return false;
  }
}

void Action::action2(Fourmi f, Place p1, Place p2)
{
  f.porteSucre();
  p1.posePheroSucre();
}

void Action::action3(Fourmi f, Place p1, Place p2)
{
  f.poseSucre();
}

void Action::action4(Fourmi f, Place p1, Place p2)
{
  deplaceFourmi(f, p1, p2);
  p2.posePheroSucre();
}

void Action::action7(Fourmi f, Place p1, Place p2)
{
  deplaceFourmi(f, p1, p2);
}

void Action::action_n(int n, Fourmi f, Place p1, Place p2)
{
  switch (n)
  {
  case 2:
    action2(f, p1, p2);
  case 3:
    action3(f, p1, p2);
  case 4:
    action4(f, p1, p2);
  case 7:
    action7(f, p1, p2);

  default:
    break;
  }
}