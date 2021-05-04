#ifndef PLACE_H
#define PLACE_H

#include "coord.hpp"
#include "fourmi.hpp"

class Place
{
private:
  Coord coord;
  float pheroSucre;
  float pheroNid;
  int numeroFourmi = -1;

public:
  Place();
  Coord getCoord() const { return coord; };

  // Les getters.
  float getPheroSucre() const { return pheroSucre; };
  float getPheroNid() const { return pheroNid; };
  int getNumeroFourmi() const { return numeroFourmi; };

  // Les méthodes générales.
  bool contientSucre() const { return pheroSucre > 0; };
  bool contientNid() const { return pheroNid == 1; };
  bool estSurUnePiste() const { return pheroNid > 0 && not contientNid(); };
  void poseSucre() { pheroSucre = 255; };
  void enleveSucre(float quantite) { pheroSucre = 0; };
  void poseNid() { pheroNid = 1; };
  void poseFourmi(Fourmi fourmi);
  void enleveFourmi() { numeroFourmi = -1; };
  void posePheroNid(float quantite);
  void posePheroSucre(float quantite);
  void diminuerPheroSucre();
};

// Les procédures.
void deplaceFourmi(Fourmi fourmi, Place p1, Place p2);

// Les prédicats.
bool estVide();
bool estPlusProcheNid();

#endif