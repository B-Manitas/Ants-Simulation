#ifndef PLACE_H
#define PLACE_H

#include "coord.hpp"
#include "fourmi.hpp"
#include <iostream>

class Place
{
private:
  Coord m_coord;
  int m_numeroFourmi;
  int m_pheroSucre;
  double m_pheroNid;
  int m_morceauSucre;
  int m_morceauNid;

public:
  // Constructor
  Place() : m_coord{Coord()}, m_numeroFourmi{-1},
            m_pheroSucre{0}, m_pheroNid{0},
            m_morceauSucre{0}, m_morceauNid{0} {};
  Place(Coord coord) : m_coord{coord}, m_numeroFourmi{-1},
                       m_pheroSucre{0}, m_pheroNid{0},
                       m_morceauSucre{0}, m_morceauNid{0} {};

  // Les getters.
  Coord getCoord() const { return m_coord; };
  double getPheroSucre() const { return m_pheroSucre; };
  double getPheroNid() const { return m_pheroNid; };
  int getNumeroFourmi() const { return m_numeroFourmi; };

  // Les méthodes générales.
  bool contientSucre() const { return m_morceauSucre > 0; };
  bool contientNid() const { return m_morceauNid > 0; };
  bool contientFourmi() const { return m_numeroFourmi != -1; };
  bool contientPheroSucre() const { return m_pheroSucre > 0; }
  bool estVide() const { return not(contientSucre() && contientNid() && contientFourmi()); }
  bool estSurUnePiste() const { return m_pheroNid > 0 && not contientNid(); };
  bool poseSucre();
  bool poseNid();
  void enleveSucre();
  bool poseFourmi(Fourmi fourmi);
  void enleveFourmi() { m_numeroFourmi = -1; };
  void posePheroNid(double quantite);
  void posePheroSucre(double quantite = 255);
  void diminuerPheroSucre();
  bool operator==(const Place &p) const;
  friend std::ostream &operator<<(std::ostream &out, const Place &place);
};

bool estPlusProcheNid(Place p1, Place p2);
void deplaceFourmi(Fourmi &fourmi, Place &p1, Place &p2);

#endif