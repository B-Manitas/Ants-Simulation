#ifndef PLACE_H
#define PLACE_H

#include <iostream>
#include "coord.hpp"
#include "fourmi.hpp"

class Place
{
private:
  Coord m_coord;
  int m_pheroSucre;
  double m_pheroNid;
  int m_numeroFourmi;

public:
  // Constructor
  Place(Coord coord) : m_coord{coord}, m_pheroSucre{0}, m_pheroNid{0}, m_numeroFourmi{-1} {};

  // Les getters.
  Coord getCoord() const { return m_coord; };
  double getPheroSucre() const { return m_pheroSucre; };
  double getPheroNid() const { return m_pheroNid; };
  int getNumeroFourmi() const { return m_numeroFourmi; };

  // Les méthodes générales.
  bool contientSucre() const { return m_pheroSucre > 0; };
  bool contientNid() const { return m_pheroNid == 1; };
  bool contientFourmis() const { return m_numeroFourmi != -1; };
  bool estSurUnePiste() const { return m_pheroNid > 0 && not contientNid(); };
  void poseSucre() { m_pheroSucre = 255; };
  void enleveSucre() { m_pheroSucre = 0; };
  void poseNid() { m_pheroNid = 1; };
  void poseFourmi(Fourmi fourmi);
  void enleveFourmi() { m_numeroFourmi = -1; };
  void posePheroNid(double quantite);
  void posePheroSucre(double quantite);
  void diminuerPheroSucre();
  bool operator==(const Place &p) const;
  friend std::ostream &operator<<(std::ostream &out, const Place &place);
};

// Les procédures.
void deplaceFourmi(Fourmi fourmi, Place p1, Place p2);

// Les prédicats.
bool estVide();
bool estPlusProcheNid(Place p1, Place p2);

#endif