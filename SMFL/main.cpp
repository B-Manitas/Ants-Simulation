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

enum class state_boxes
{
  fourmi,
  fourmi_sucre,
  nid,
  sucre,
  pheroSucre,
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

      if (p.contientNid())
        grid[y].push_back(int(state_boxes::nid));

      else if (p.contientSucre())
        grid[y].push_back(int(state_boxes::sucre));

      else if (p.contientFourmi() && not f.chercheSucre())
        grid[y].push_back(int(state_boxes::fourmi_sucre));

      else if (p.contientFourmi() && f.chercheSucre())
        grid[y].push_back(int(state_boxes::fourmi));

      else if (p.contientPheroSucre())
        grid[y].push_back(int(state_boxes::pheroSucre));

      else
        grid[y].push_back(int(state_boxes::vide));
    }

  return grid;
}

int main(int argc, const char **argv)
{
  srand((unsigned)time(NULL));
  // Infrastructure docstest.
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  // Déclaration des variables.
  const int TAILLE_CASE = 10;
  const int INTERVAL_TIME = 5;
  int n_iteration = 0;
  int time = 100;
  bool isPlaying = false;

  // Création des Grilles.
  Grille laGrille(TAILLEGRILLE);
  GrilleFourmis lesFourmis;
  initialiserEmplacements(laGrille, lesFourmis);
  testCoherence(laGrille, lesFourmis, "Initialisation Grille");
  std::vector<std::vector<int>> grid_state = getValueGrille(laGrille, lesFourmis);

  // Gestion de l'interface grphique.
  sf::RenderWindow window(sf::VideoMode(TAILLEGRILLE * TAILLE_CASE, TAILLEGRILLE * TAILLE_CASE + 60),
                          "Ants Simulators by Manitas Bahri",
                          sf::Style::Close | sf::Style::Titlebar);

  // Gestion de l'affichage des textes.
  sf::Font font;
  if (!font.loadFromFile("asset/arial.ttf"))
  {
    std::cout << "Failed to load the font." << std::endl;
  }

  sf::Text txt_pause("Press 'Space' to play/pause.", font);
  txt_pause.setCharacterSize(15);
  txt_pause.setPosition(10, TAILLE_CASE * TAILLEGRILLE + 10);
  txt_pause.setFillColor(sf::Color::White);

  sf::Text txt_mouse("Use left/right mouse click to add and remove a sugar.", font);
  txt_mouse.setCharacterSize(15);
  txt_mouse.setPosition(10, TAILLE_CASE * TAILLEGRILLE + 25);
  txt_mouse.setFillColor(sf::Color::White);

  sf::Text txt_arrow("Use left/right arrow keys to change the speed time.", font);
  txt_arrow.setCharacterSize(15);
  txt_arrow.setPosition(10, TAILLE_CASE * TAILLEGRILLE + 40);
  txt_arrow.setFillColor(sf::Color::White);

  sf::Color color_rect(sf::Color::White);

  while (window.isOpen())
  {
    sf::Event w_event;

    while (window.pollEvent(w_event))
    {
      switch (w_event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::KeyPressed:
        switch (w_event.key.code)
        {
        case sf::Keyboard::Space:
          isPlaying = !isPlaying;
          break;

        case sf::Keyboard::Left:
          time = std::max(time - INTERVAL_TIME, 0);
          break;

        case sf::Keyboard::Right:
          time = std::min(time + INTERVAL_TIME, 1000);
          break;

        default:
          break;
        }
        break;

      case sf::Event::MouseButtonPressed:
        if (not isPlaying && w_event.mouseButton.button == sf::Mouse::Left)
        {
          int x = double(w_event.mouseButton.x) / TAILLE_CASE;
          int y = double(w_event.mouseButton.y) / TAILLE_CASE;
          if (y > 0 and x > 0 and y < TAILLEGRILLE and x < TAILLEGRILLE)
          {
            Place mouse_place = laGrille.chargePlace(Coord(x, y));
            if (mouse_place.poseSucre())
            {
              grid_state[y][x] = int(state_boxes::sucre);
              laGrille.rangePlace(mouse_place);
            }
          }
        }

        else if (not isPlaying && w_event.mouseButton.button == sf::Mouse::Right)
        {
          int x = double(w_event.mouseButton.x) / TAILLE_CASE;
          int y = double(w_event.mouseButton.y) / TAILLE_CASE;
          if (y > 0 and x > 0 and y < TAILLEGRILLE and x < TAILLEGRILLE)
          {
            Place mouse_place = laGrille.chargePlace(Coord(x, y));
            mouse_place.enleveSucre();
            mouse_place.enlevePheroSucre();
            grid_state[y][x] = int(state_boxes::vide);
            laGrille.rangePlace(mouse_place);
          }
        }
        break;

      default:
        break;
      }
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
          color_rect = sf::Color::Yellow;

        else if (grid_state[y][x] == 2)
          color_rect = sf::Color(155, 155, 155);

        else if (grid_state[y][x] == 3)
          color_rect = sf::Color::White;

        else if (grid_state[y][x] == 4)
          color_rect = sf::Color(place.getPheroSucre(), place.getPheroSucre(), 0);

        else
          color_rect = sf::Color::Black;

        rect.setFillColor(color_rect);
        window.draw(rect);
      }

    if (isPlaying)
    {
      mettreAJourEnsFourmis(laGrille, lesFourmis);
      testCoherence(laGrille, lesFourmis, "Simulation " + std::to_string(n_iteration));
      laGrille.diminuePheroSucre();
      grid_state = getValueGrille(laGrille, lesFourmis);
    }

    window.draw(txt_pause);
    window.draw(txt_mouse);
    window.draw(txt_arrow);
    window.display();

    sf::sleep(sf::milliseconds(time));
    n_iteration++;
  }

  return 0;
}
