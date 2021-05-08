#ifndef Fourmi_HPP
#define Fourmi_HPP
#include "coord.hpp"
// #include "grille.hpp"

class Fourmi
{
private:
  Coord m_coord;
  int m_num;
  bool m_contientSucre;

public:
  Fourmi() : m_coord{Coord()}, m_num{-1} {};
  Fourmi(Coord c, int numero);
  Coord getCoord() const { return m_coord; };
  int getNum() const { return m_num; };
  bool getContientSucre() const { return m_contientSucre; };
  void setContientSucre(bool contientSucre) { m_contientSucre = contientSucre; }
  void porteSucre() { m_contientSucre = true; }
  void poseSucre() { m_contientSucre = false; }
  bool operator==(const Fourmi &f) const;
};

// void mettreAJourUneFourmi(Fourmi fourmi, Grille laGrille) {}
// void mettreAJourEnsFourmis(Grille laGrille, std::vector<Fourmi> lesFourmis);

#endif