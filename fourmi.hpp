#ifndef Fourmi_HPP
#define Fourmi_HPP
#include "coord.hpp"

class Fourmi
{
private:
  Coord m_coord;
  int m_num;

public:
  Fourmi(Coord c, int numero) : m_coord{c}, m_num{numero} {};
  Coord getCoord() const { return m_coord; };
  int getNum() const { return m_num; };
  bool porteSucre() const;

  void prendSucre();
  void poseSucre();
  void deplace();
};

#endif