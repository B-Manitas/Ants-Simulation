#include "Ant.hpp"
#include "Coord.hpp"
#include "Grid.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"
#include "time.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * Reset the state of the simulation
 * @param grid The grid.
 * @param ants The grid of the ants.
 * @param set_nests The set of nests.
 * @param grid_state The state of each square.
 * @param iteration The number of iteration.
 * @param is_sandbox Set the simulation to sanbox mode.
*/
void reset(Grid &grid, GridAnts &ants, EnsCoord &set_nests,
           std::vector<std::vector<int>> &grid_state,
           int &iteration, bool is_sandbox)
{
  grid = Grid(GRID_SIZE);
  ants = GridAnts();
  set_nests = EnsCoord();
  initializeRandomPlaces(grid, ants, set_nests, NB_SUGAR, NB_ANT, not is_sandbox);
  consistencyTest(grid, ants, "Grid Initialization");
  grid_state = getGridState(grid, ants);
  iteration = 0;
}

int main()
{
  srand((unsigned)time(NULL));

  // Variables declaration.

  bool is_playing = false;
  bool is_edit_mode = false;
  bool is_decimated = false;
  bool is_enough_food = true;
  int speed_time = INITIAL_SPEED_TIME;
  int iteration = 0;
  int reserve_sugar = 0;

  // Creation of grids
  Grid grid = Grid(GRID_SIZE);
  GridAnts ants;
  EnsCoord set_nests;
  std::vector<std::vector<int>> grid_state;

  reset(grid, ants, set_nests, grid_state, iteration, false);

  // Create the application window.
  sf::RenderWindow window(sf::VideoMode(GRID_SIZE * SQUARE_SIZE, GRID_SIZE * SQUARE_SIZE + 100),
                          "Ants Simulators by Manitas Bahri",
                          sf::Style::Close | sf::Style::Titlebar);

  // Create the texts.
  sf::Font font;
  if (not font.loadFromFile("./asset/Gameplay.ttf"))
    std::cout << "Warning Font: Failed to load the font." << std::endl;

  sf::Text txt_days;
  txt_days.setFont(font);
  txt_days.setCharacterSize(15);
  txt_days.setPosition(15, SQUARE_SIZE * GRID_SIZE);
  txt_days.setFillColor(sf::Color::White);

  sf::Text txt_nb_ants;
  txt_nb_ants.setFont(font);
  txt_nb_ants.setCharacterSize(15);
  txt_nb_ants.setPosition(15, SQUARE_SIZE * GRID_SIZE + 25);
  txt_nb_ants.setFillColor(sf::Color::White);

  sf::Text txt_reserve;
  txt_reserve.setFont(font);
  txt_reserve.setCharacterSize(15);
  txt_reserve.setPosition(15, SQUARE_SIZE * GRID_SIZE + 50);
  txt_reserve.setFillColor(sf::Color::White);

  sf::Text txt_nests_size;
  txt_nests_size.setFont(font);
  txt_nests_size.setCharacterSize(15);
  txt_nests_size.setPosition(15, SQUARE_SIZE * GRID_SIZE + 75);
  txt_nests_size.setFillColor(sf::Color::White);

  sf::Text txt_state;
  txt_state.setFont(font);
  txt_state.setCharacterSize(50);
  txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 220, SQUARE_SIZE * GRID_SIZE + 30);
  txt_state.setFillColor(sf::Color::White);

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
        break;

      // Shortcut keyboard.
      case sf::Event::KeyPressed:
        if (app_event.key.code == sf::Keyboard::Space)
          is_playing = !is_playing;

        // Increase the speed time.
        else if (app_event.key.code == sf::Keyboard::Left and speed_time < MAX_TIME)
          speed_time = std::min(speed_time + INTERVAL_TIME, MAX_TIME);

        // Decrease the speed time.
        else if (app_event.key.code == sf::Keyboard::Right and speed_time > MIN_TIME)
          speed_time = std::max(speed_time - INTERVAL_TIME, MIN_TIME);

        // Stop the simulation.
        else if (app_event.key.code == sf::Keyboard::Q)
          window.close();

        // Reset the simulation.
        else if (app_event.key.code == sf::Keyboard::R and not is_playing)
        {
          reset(grid, ants, set_nests, grid_state, iteration, is_edit_mode);
        }

        // Active edit mode.
        else if (app_event.key.code == sf::Keyboard::Num0 and not is_playing)
        {
          is_edit_mode = true;
          reset(grid, ants, set_nests, grid_state, iteration, is_edit_mode);
        }

        // Active edit mode.
        else if (app_event.key.code == sf::Keyboard::Num1 and not is_playing)
        {
          is_edit_mode = false;
          reset(grid, ants, set_nests, grid_state, iteration, is_edit_mode);
        }
        break;

      case sf::Event::MouseButtonPressed:
        // Remove/Add a ant.
        if (not is_playing && app_event.mouseButton.button == sf::Mouse::Left)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y >= 0 and x >= 0 and y <= GRID_SIZE and x <= GRID_SIZE)
          {
            Place place_clicked = grid.getPlace(Coord(x, y));

            Ant ant_clicked = place_clicked.isContainingAnt() ? ants.getAnt(Coord(x, y)) : Ant(Coord(x, y), ants.getNewIndex());
            if (place_clicked.isContainingAnt() && ants.isContainingAntsCoord(Coord(x, y)))
            {
              place_clicked.removeAnt();
              ants.remove(ant_clicked);
              grid_state[y][x] = int(place_state::empty);
            }

            else if (place_clicked.isEmpty() and not place_clicked.isContainingWater() and place_clicked.putAnt(ant_clicked))
            {
              ants.setAnt(ant_clicked);
              grid_state[y][x] = int(place_state::ant);
            }

            grid.setPlace(place_clicked);
          }
        }

        // Remove/Add a sugar.
        else if (not is_playing && app_event.mouseButton.button == sf::Mouse::Right)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y >= 0 and x >= 0 and y <= GRID_SIZE and x <= GRID_SIZE)
          {
            Place place_clicked = grid.getPlace(Coord(x, y));

            if (place_clicked.isContainingSugar())
            {
              place_clicked.removeSugar();
              place_clicked.removePheroSugar();
              grid_state[y][x] = int(place_state::empty);
              grid.setPlace(place_clicked);
            }

            else if (place_clicked.isEmpty() and not place_clicked.isContainingWater())
            {
              place_clicked.putSugar();
              grid_state[y][x] = int(place_state::sugar);
              grid.setPlace(place_clicked);
            }
          }
        }

        // Add/remove water.
        else if (not is_playing && app_event.mouseButton.button == sf::Mouse::Middle)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y >= 0 and x >= 0 and y <= GRID_SIZE and x <= GRID_SIZE)
          {
            Place place = grid.getPlace(Coord(x, y));
            if (place.isContainingWater())
            {
              place.removeWater();
              grid.setPlace(place);
              grid_state[y][x] = int(place_state::empty);
            }

            else if (place.putWater())
            {
              grid.setPlace(place);
              grid_state[y][x] = int(place_state::water);
            }
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
        Place place = grid.getPlace(Coord(x, y));

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

        case int(place_state::water):
          color_rect = sf::Color(119, 181, 254);
          break;

        case int(place_state::phero_sugar):
          color_rect = sf::Color(place.getPheroSugar(), place.getPheroSugar(), 0);
          break;

        default:
          color_rect = sf::Color::Black;
          break;
        }

        rect.setFillColor(color_rect);
        window.draw(rect);
      }

    // Update the next grid of the simulation.
    if (is_playing)
    {
      updateSetAnts(grid, ants);
      consistencyTest(grid, ants, "Simulation " + std::to_string(iteration));
      grid.decreasePheroSugar(3);

      if (ants.getSize() > 0)
        is_decimated = not evolution(grid, ants, set_nests, is_enough_food, reserve_sugar, iteration, 20, 10);

      grid_state = getGridState(grid, ants);
      // Pause the simulation.
      iteration++;
      sf::sleep(sf::milliseconds(speed_time));
    }

    // Set the text values.
    txt_days.setString("Generation: " + std::to_string(iteration));
    txt_nb_ants.setString("Number of Ants: " + std::to_string(ants.getSize()));
    txt_nests_size.setString("Nest Size: " + std::to_string(set_nests.getSize()));
    txt_reserve.setString("Reserve Sugar: " + std::to_string(std::max(reserve_sugar, 0)));

    // Format the speed time text
    std::string format_time = std::to_string((MAX_TIME - speed_time) / 100.);
    format_time = format_time.substr(0, format_time.find(".") + 3);
    txt_nests_size.setString("Speed Time: " + format_time);

    // Set the text values for the simulation state.
    if (not is_playing and is_edit_mode)
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 290, SQUARE_SIZE * GRID_SIZE + 30);
      txt_state.setString("Sandbox");
      txt_state.setFillColor(sf::Color::White);
      txt_state.setCharacterSize(50);
    }

    else if (not is_playing)
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 220, SQUARE_SIZE * GRID_SIZE + 30);
      txt_state.setString("Pause");
      txt_state.setFillColor(sf::Color::White);
      txt_state.setCharacterSize(50);
    }

    else if (is_decimated)
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 410, SQUARE_SIZE * GRID_SIZE + 45);
      txt_state.setString("Decimated Colony");
      txt_state.setFillColor(sf::Color::Red);
      txt_state.setCharacterSize(35);
    }

    else if (ants.getSize() > LIMITS_ANT)
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 400, SQUARE_SIZE * GRID_SIZE + 30);
      txt_state.setString("Overcrowding");
      txt_state.setFillColor(sf::Color::Yellow);
      txt_state.setCharacterSize(40);
    }

    else if (not is_enough_food)
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 240, SQUARE_SIZE * GRID_SIZE + 30);
      txt_state.setString("Famine");
      txt_state.setFillColor(sf::Color::Yellow);
      txt_state.setCharacterSize(50);
    }

    else
    {
      txt_state.setPosition(SQUARE_SIZE * GRID_SIZE - 350, SQUARE_SIZE * GRID_SIZE + 30);
      txt_state.setString("Enough Food");
      txt_state.setFillColor(sf::Color::White);
      txt_state.setCharacterSize(40);
    }

    // Draw all elements of the windows.
    window.draw(txt_days);
    window.draw(txt_nb_ants);
    window.draw(txt_reserve);
    window.draw(txt_nests_size);
    window.draw(txt_state);
    window.display();
  }
  return 0;
}
