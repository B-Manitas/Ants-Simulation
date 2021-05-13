#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "main.hpp"
#include "coord.hpp"
#include "place.hpp"
#include "grille.hpp"
#include "grilleFourmis.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define WHITE sf::Color::White
#define BLACK sf::Color::Black
#define GREEN sf::Color::Green
#define GREEN_LIGHT sf::Color(0, 155, 0)
#define GRAY sf::Color(153, 153, 153)

enum class state_boxes
{
  fourmi,
  fourmi_sucre,
  nid,
  sucre,
  vide
};

void testCoherence(Grille &laGrille, GrilleFourmis &lesFourmis, std::string title)
{
  std::ostringstream error_msg;

  for (auto &f : lesFourmis.m_grilleF)
  {
    Place pF = laGrille.chargePlace(f.getCoord());

    if (pF.getNumeroFourmi() != f.getNum())
    {
      error_msg << title << std::endl;
      error_msg << "Erreur Incoherence: L'indice d'une fourmi ne correspond pas à sa place." << std::endl
                << "Fourmis: " << f << std::endl
                << "Place: " << pF << std::endl;

      throw std::runtime_error(error_msg.str());
    }
  }

  for (int y = 0; y < laGrille.taille(); y++)
    for (int x = 0; x < laGrille.taille(); x++)
    {
      Place p = laGrille.chargePlace(Coord(x, y));

      if (p.contientFourmi() && not lesFourmis.contientFourmis(p.getNumeroFourmi()))
      {
        error_msg << title << std::endl;
        error_msg << "Erreur Incoherence: Une place contient une fourmi qui n'existe pas." << std::endl
                  << "Place: " << p << std::endl
                  << "Les Fourmis: " << lesFourmis << std::endl;

        throw std::runtime_error(error_msg.str());
      }
    }
}

std::vector<std::vector<int>> getValueGrille(Grille &laGrille, GrilleFourmis &lesFourmis)
{
  std::vector<std::vector<int>> grid(TAILLEGRILLE);
  for (int y = 0; y < laGrille.taille(); y++)
    for (int x = 0; x < laGrille.taille(); x++)
    {
      Coord c = Coord(x, y);
      Place p = laGrille.chargePlace(c);
      Fourmi f = Fourmi();

      if (lesFourmis.contientFourmisCoord(c))
        f = lesFourmis.chargeFourmi(c);

      if (p.contientFourmi() && f.chercheSucre())
        grid[y].push_back(int(state_boxes::fourmi));

      else if (p.contientFourmi())
        grid[y].push_back(int(state_boxes::fourmi_sucre));

      else if (p.contientNid())
        grid[y].push_back(int(state_boxes::nid));

      else if (p.contientSucre())
        grid[y].push_back(int(state_boxes::sucre));

      else
        grid[y].push_back(int(state_boxes::vide));
    }

  return grid;
}

int main(int argc, const char **argv)
{
  // Infrastructure docstest.
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  // Déclaration des variables.
  const int TAILLE_CASE = 10;
  int temps_pause = 100;

  // Création des Grilles.
  Grille laGrille(TAILLEGRILLE);
  GrilleFourmis lesFourmis;
  initialiserEmplacements(laGrille, lesFourmis);
  testCoherence(laGrille, lesFourmis, "Initialisation Grille");
  std::vector<std::vector<int>> grid_state = getValueGrille(laGrille, lesFourmis);

  // Gestion de l'interface grphique.
  sf::RenderWindow window(sf::VideoMode(TAILLEGRILLE * TAILLE_CASE, TAILLEGRILLE * TAILLE_CASE),
                          "Ants Simulators by Manitas Bahri",
                          sf::Style::Close | sf::Style::Titlebar);

  sf::Color color_rect(sf::Color::Black);

  while (window.isOpen())
  {
    sf::Event w_event;

    while (window.pollEvent(w_event))
    {
      if (w_event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    for (int y = 0; y < TAILLEGRILLE; y++)
      for (int x = 0; x < TAILLEGRILLE; x++)
      {
        // Dessine une case.
        sf::RectangleShape rect;
        rect.setPosition(x * TAILLE_CASE, y * TAILLE_CASE);
        rect.setSize(sf::Vector2f(TAILLE_CASE, TAILLE_CASE));

        Place place = laGrille.chargePlace(Coord(x, y));

        if (grid_state[y][x] == 0)
          color_rect = sf::Color::Yellow;

        else if (grid_state[y][x] == 1)
          color_rect = sf::Color(std::max(place.getPheroSucre(), 250.), std::max(place.getPheroSucre(), 250.), 0);

        else if (grid_state[y][x] == 2)
          color_rect = sf::Color(155, 155, 155);

        else if (grid_state[y][x] == 3)
          color_rect = sf::Color::White;

        else
          color_rect = sf::Color::Black;

        rect.setFillColor(color_rect);
        window.draw(rect);
      }

    window.display();

    mettreAJourEnsFourmis(laGrille, lesFourmis);
    testCoherence(laGrille, lesFourmis, "Simulation ");
    laGrille.diminuePheroSucre();
    grid_state = getValueGrille(laGrille, lesFourmis);

    sf::sleep(sf::milliseconds(temps_pause));
  }

  return 0;
}
