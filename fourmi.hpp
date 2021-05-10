#ifndef Fourmi_HPP
#define Fourmi_HPP
#include "coord.hpp"
// #include "place.hpp"
// #include "grille.hpp"

class Fourmi
{
private:
  Coord m_coord;
  int m_num;
  bool m_contientSucre;

public:
  Fourmi() : m_coord{Coord()}, m_num{-1}, m_contientSucre{false} {};
  Fourmi(Coord c, int numero);
  Coord getCoord() const { return m_coord; };
  int getNum() const { return m_num; };
  bool chercheSucre() const { return not m_contientSucre; };
  void porteSucre() { m_contientSucre = true; }
  void poseSucre() { m_contientSucre = false; }
  void deplace(Coord const &c) { m_coord = c; };
  bool operator==(const Fourmi &f) const;
  friend std::ostream &operator<<(std::ostream &out, const Fourmi &f);
};

#endif