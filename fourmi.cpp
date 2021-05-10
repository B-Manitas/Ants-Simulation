#include "fourmi.hpp"
#include "coord.hpp"
#include "doctest.h"

Fourmi::Fourmi(Coord c, int numero) : m_coord{c}, m_num{numero}, m_contientSucre{false}
{
  if (m_num < 0 and m_num != -1)
    throw std::runtime_error("Le numéro de la fourmi doit être positif. ");
}

bool Fourmi::operator==(const Fourmi &f) const
{
  return f.getCoord() == m_coord && f.getNum() == m_num;
}

std::ostream &operator<<(std::ostream &out, const Fourmi &f)
{
  out << "{ Coord: " << f.getCoord()
      << ", Num: " << f.getNum()
      << ", Sucre: " << ((f.chercheSucre() == true) ? "Non" : "Oui")
      << " }";

  return out;
}
