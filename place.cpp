#include "place.hpp"

void Place::posePheroSucre(float quantite)
{
  if (quantite > 0)
    pheroSucre += (quantite > 255) ? 255 : quantite;
};

void Place::posePheroNid(float quantite)
{
  if (quantite > 0)
    pheroSucre += (quantite > 1) ? 1 : quantite;
};

void Place::poseFourmi(Fourmi fourmi)
{
  if (numeroFourmi == -1)
    numeroFourmi = fourmi.getNum();
};

void Place::diminuerPheroSucre()
{
  if (pheroSucre > 0)
    pheroSucre -= 5;
};

/**
 * Déplace la fourmi de la place p1 vers la place p2.
 * @param fourmis La fourmis à déplacer.
 * @param p1 La place actuelle.
 * @param p2 La nouvelle place de la fourmi.
*/
void deplaceFourmi(Fourmi fourmi, Place p1, Place p2)
{
  p1.enleveFourmi();
  p2.poseFourmi(fourmi);
}

/**
 * Revoie true si la place est vide.
*/
bool estVide(Place p)
{
  return p.getNumeroFourmi() == -1 && (not p.contientNid()) && (not p.contientSucre());
}

/**
 * Renvoie true si p1 est plus proche du nid que p2.
*/
bool estPlusProcheNid(Place p1, Place p2) { return p1.getPheroNid() > p2.getPheroNid(); }
