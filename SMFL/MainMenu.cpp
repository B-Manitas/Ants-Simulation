#include "MainMenu.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "Grid.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"
#include "time.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>

MainMenu::MainMenu() : m_speed_time{INITIAL_SPEED_TIME}, m_iteration{0}, m_is_playing{false}
{
  // Creation of grids
  Grid grid = Grid(GRID_SIZE);
  GridAnts ants;
  EnsCoord set_nests;

  initializeRandomPlaces(grid, ants, set_nests, NB_SUGAR, NB_ANT);
  consistencyTest(grid, ants, "Grid Initialization");
  std::vector<std::vector<int>> grid_state = getGridState(grid, ants);

  // Create the application window.
  sf::RenderWindow window(sf::VideoMode(GRID_SIZE * SQUARE_SIZE, GRID_SIZE * SQUARE_SIZE + 60),
                          "Ants Simulators by Manitas Bahri",
                          sf::Style::Close | sf::Style::Titlebar);

  // Create the texts.
  sf::Font font;
  if (!font.loadFromFile("./asset/arial.ttf"))
    std::cout << "Warning Font: Failed to load the font." << std::endl;

  sf::Text txt_pause;
  txt_pause.setFont(font);
  txt_pause.setCharacterSize(15);
  txt_pause.setPosition(10, SQUARE_SIZE * GRID_SIZE + 10);
  txt_pause.setFillColor(sf::Color::White);

  sf::Text txt_mouse("Right click to toggled sugar/no sugar.", font);
  txt_mouse.setCharacterSize(15);
  txt_mouse.setPosition(10, SQUARE_SIZE * GRID_SIZE + 25);
  txt_mouse.setFillColor(sf::Color::White);

  sf::Text txt_arrow("Left  click to toggled ant/no ant.", font);
  txt_arrow.setCharacterSize(15);
  txt_arrow.setPosition(10, SQUARE_SIZE * GRID_SIZE + 40);
  txt_arrow.setFillColor(sf::Color::White);

  while (window.isOpen())
  {
    sf::Event app_event;

    while (window.pollEvent(app_event))
    {
      switch (app_event.type)
      {
      // Closed window.
      case sf::Event::Closed:
        window.close();
        std::cout << "Iteration: " << m_iteration << std::endl;
        std::cout << "Stop the simulation." << std::endl;
        break;

      // Shortcut keyboard.
      case sf::Event::KeyPressed:
        if (app_event.key.code == sf::Keyboard::Space)
          m_is_playing = !m_is_playing;

        // Increase the speed time.
        else if (app_event.key.code == sf::Keyboard::Left and m_speed_time < MAX_TIME)
        {
          m_speed_time = std::min(m_speed_time + INTERVAL_TIME, MAX_TIME);
          std::cout << "Time: " << time << std::endl;
        }

        // Decrease the speed time.
        else if (app_event.key.code == sf::Keyboard::Right and m_speed_time > MIN_TIME)
        {
          m_speed_time = std::max(m_speed_time - INTERVAL_TIME, MIN_TIME);
          std::cout << "Time: " << time << std::endl;
        }

        // Stop the simulation.
        else if (app_event.key.code == sf::Keyboard::Q)
        {
          window.close();
          std::cout << "Iteration: " << m_iteration << std::endl;
          std::cout << "Stop the simulation." << std::endl;
        }

        // Reset the simulation.
        else if (app_event.key.code == sf::Keyboard::R and not m_is_playing)
        {
          grid = Grid(GRID_SIZE);
          ants = GridAnts();
          set_nests = EnsCoord();
          initializeRandomPlaces(grid, ants, set_nests, NB_SUGAR, NB_ANT);
          consistencyTest(grid, ants, "Grid Initialization after Reset");
          grid_state = getGridState(grid, ants);
          m_iteration = 0;
          std::cout << "The simulation is reset." << std::endl;
        }
        break;

      case sf::Event::MouseButtonPressed:
        if (not m_is_playing && app_event.mouseButton.button == sf::Mouse::Left)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y > 0 and x > 0 and y < GRID_SIZE and x < GRID_SIZE)
          {
            Place place_clicked = grid.getPlace(Coord(x, y));

            Ant ant_clicked = place_clicked.isContainingAnt() ? ants.getAnt(Coord(x, y)) : Ant(Coord(x, y), ants.getNewIndex());
            if (place_clicked.isContainingAnt() && ants.isContainingAntsCoord(Coord(x, y)))
            {
              place_clicked.removeAnt();
              ants.remove(ant_clicked);
              grid_state[y][x] = int(place_state::empty);
            }

            else if (place_clicked.isEmpty() and place_clicked.putAnt(ant_clicked))
            {
              ants.setAnt(ant_clicked);
              grid_state[y][x] = int(place_state::ant);
            }

            grid.setPlace(place_clicked);
          }
        }

        else if (not m_is_playing && app_event.mouseButton.button == sf::Mouse::Right)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y > 0 and x > 0 and y < GRID_SIZE and x < GRID_SIZE)
          {
            Place place_clicked = grid.getPlace(Coord(x, y));

            if (place_clicked.isContainingSugar())
            {
              place_clicked.removeSugar();
              place_clicked.removePheroSugar();
              grid_state[y][x] = int(place_state::empty);
              grid.setPlace(place_clicked);
            }

            else if (place_clicked.isEmpty())
            {
              place_clicked.putSugar();
              grid_state[y][x] = int(place_state::sugar);
              grid.setPlace(place_clicked);
            }
          }
        }

        else if (not m_is_playing && app_event.mouseButton.button == sf::Mouse::Middle)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y > 0 and x > 0 and y < GRID_SIZE and x < GRID_SIZE)
          {
            std::cout << grid.getPlace(Coord(x, y)) << std::endl;
          }
        }
        break;

      default:
        break;
      }
    }

    window.clear();
    for (int y = 0; y < GRID_SIZE; y++)
      for (int x = 0; x < GRID_SIZE; x++)
      {
        Place square = grid.getPlace(Coord(x, y));

        // Create a square.
        sf::RectangleShape rect;
        rect.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
        rect.setSize(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        sf::Color color_rect;

        // Colorize each square according to its state.
        switch (grid_state[y][x])
        {
        case int(place_state::ant):
          color_rect = sf::Color::Yellow;
          break;

        case int(place_state::nest):
          color_rect = sf::Color(155, 155, 155);
          break;

        case int(place_state::sugar):
          color_rect = sf::Color::White;
          break;

        case int(place_state::phero_sugar):
          color_rect = sf::Color(square.getPheroSugar(), square.getPheroSugar(), 0);
          break;

        default:
          color_rect = sf::Color::Black;
          break;
        }

        rect.setFillColor(color_rect);
        window.draw(rect);
      }

    // Update the next grid of the simulation.
    if (m_is_playing)
    {
      // Add a sugar every 100 iterations.
      if (m_iteration % 100 == 0)
      {
        Place rand_sugar_place = grid.getRandEmptyPlace();

        if (rand_sugar_place.putSugar())
        {
          grid.setPlace(rand_sugar_place);
          std::cout << "A new pile of sugar appears." << std::endl;
        }
      }

      updateSetAnts(grid, ants);
      consistencyTest(grid, ants, "Simulation " + std::to_string(m_iteration));
      grid.decreasePheroSugar(3);
      grid_state = getGridState(grid, ants);
      evolution(grid, ants, set_nests, 20, 10);
      txt_pause.setString("Space to pause.");

      // Pause the simulation.
      m_iteration++;
      sf::sleep(sf::milliseconds(m_speed_time));
    }
    else
      txt_pause.setString("Space to play.");

    // Draw all elements of the windows.
    window.draw(txt_pause);
    window.draw(txt_mouse);
    window.draw(txt_arrow);
    window.display();
  }
}