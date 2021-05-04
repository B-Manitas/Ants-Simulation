#ifndef Fourmi_HPP
#define Fourmi_HPP
#include "coord.hpp"

class Fourmi
{
private:
  Coord coord;
  int num;

public:
  Fourmi(Coord c, int numero) : coord{c}, num{numero} {};
  Coord getCoord() const { return coord; };
  int getNum() const { return num; };
  bool porteSucre() const;

  void prendSucre();
  void poseSucre();
  void deplace();
};

#endif