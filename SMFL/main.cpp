#include "Ant.hpp"
#include "Coord.hpp"
#include "Grid.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>

enum class place_state
{
  ant,
  nest,
  sugar,
  phero_sugar,
  empty
};

/**
 * Test if the simulation is consistent with the grid and the position of the ants.
 * Throw an exception if an inconsistency is found.
 * @param grid The grid containing all the simulation squares.
 * @param ants The grid containing all the ants in the simulation.
*/
void consistencyTest(Grid &grid, GridAnts &ants, std::string title)
{
  std::ostringstream error_msg;

  // Take the first test. Test if the ant's index and square match.
  for (auto &ant : ants.m_grid)
  {
    Place square = grid.getPlace(ant.getCoord());

    if (square.getIdAnt() != ant.getIndex())
    {
      error_msg << title << std::endl;
      error_msg << "Inconsistent Simulation Error: The index of an ant does not correspond to its square." << std::endl
                << "Ant: " << ant << std::endl
                << "Square: " << square << std::endl;

      throw std::runtime_error(error_msg.str());
    }
  }

  // Take the second test. Test if the square has a ghost ant.
  for (size_t y = 0; y < grid.getSize(); y++)
    for (size_t x = 0; x < grid.getSize(); x++)
    {
      Place square = grid.getPlace(Coord(x, y));

      if (square.isContainingAnt() && not ants.isContainingAnts(square.getIdAnt()))
      {
        error_msg << title << std::endl;
        error_msg << "Inconsistent Simulation Error: A square contains an ant that doesn't exist." << std::endl
                  << "Square: " << square << std::endl
                  << "Ants: " << ants << std::endl;

        throw std::runtime_error(error_msg.str());
      }
    }
}

/**
 * Create a 2D vector with the state of each square.
 * @param grid The grid containing all the simulation squares.
 * @param ants The grid containing all the ants in the simulation.
 * @return Returns a 2D vector of integer corresponding to the state of the square.
*/
std::vector<std::vector<int>> getGridState(Grid &grid, GridAnts &ants)
{
  std::vector<std::vector<int>> color_grid(GRID_SIZE);

  for (size_t y = 0; y < grid.getSize(); y++)
    for (size_t x = 0; x < grid.getSize(); x++)
    {
      Coord square_coord = Coord(x, y);
      Place square = grid.getPlace(square_coord);
      Ant ant = Ant();

      if (ants.isContainingAntsCoord(square_coord))
        ant = ants.getAnt(square_coord);

      // Square containing an ant nest.
      if (square.isContainingAntNest())
        color_grid[y].push_back(int(place_state::nest));

      // Square containing some sugars.
      else if (square.isContainingSugar())
        color_grid[y].push_back(int(place_state::sugar));

      // Square containing an ant.
      else if (square.isContainingAnt())
        color_grid[y].push_back(int(place_state::ant));

      // Square containing sugar pheromone.
      else if (square.isContainingPheroSugar())
        color_grid[y].push_back(int(place_state::phero_sugar));

      // The square is empty.
      else
        color_grid[y].push_back(int(place_state::empty));
    }

  return color_grid;
}

int main()
{
  srand((unsigned)time(NULL));

  // Variables declaration.
  const int SQUARE_SIZE = 10;
  const int INTERVAL_TIME = 5;
  int iteration = 0;
  int time = 100;
  bool is_playing = false;

  // Creation of grids
  Grid grid = Grid(GRID_SIZE);
  GridAnts ants;
  EnsCoord set_nests;

  initializeRandomPlaces(grid, ants, set_nests);
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
        std::cout << "Iteration: " << iteration << std::endl;
        std::cout << "Stop the simulation." << std::endl;
        break;

      // Shortcut keyboard.
      case sf::Event::KeyPressed:
        if (app_event.key.code == sf::Keyboard::Space)
          is_playing = !is_playing;

        // Increase the speed time.
        else if (app_event.key.code == sf::Keyboard::Left)
        {
          time = std::max(time + INTERVAL_TIME, 0);
          std::cout << "Time: " << time << std::endl;
        }

        // Decrease the speed time.
        else if (app_event.key.code == sf::Keyboard::Right)
        {
          time = std::min(time - INTERVAL_TIME, 1000);
          std::cout << "Time: " << time << std::endl;
        }

        // Stop the simulation.
        else if (app_event.key.code == sf::Keyboard::Q)
        {
          window.close();
          std::cout << "Iteration: " << iteration << std::endl;
          std::cout << "Stop the simulation." << std::endl;
        }

        // Reset the simulation.
        else if (app_event.key.code == sf::Keyboard::R and not is_playing)
        {
          grid = Grid(GRID_SIZE);
          ants = GridAnts();
          set_nests = EnsCoord();
          initializeRandomPlaces(grid, ants, set_nests);
          consistencyTest(grid, ants, "Grid Initialization after Reset");
          grid_state = getGridState(grid, ants);
          iteration = 0;
          std::cout << "The simulation is reset." << std::endl;
        }
        break;

      case sf::Event::MouseButtonPressed:
        if (not is_playing && app_event.mouseButton.button == sf::Mouse::Left)
        {
          int x = double(app_event.mouseButton.x) / SQUARE_SIZE;
          int y = double(app_event.mouseButton.y) / SQUARE_SIZE;

          if (y > 0 and x > 0 and y < GRID_SIZE and x < GRID_SIZE)
          {
            Place place_clicked = grid.getPlace(Coord(x, y));

            Ant ant_clicked = place_clicked.isContainingAnt() ? ants.getAnt(Coord(x, y)) : Ant(Coord(x, y), ants.getNewIndex());
            if (place_clicked.isContainingAnt() && ants.isContainingAntsCoord(Coord(x, y)))
            {
              ants.remove(ant_clicked);
              place_clicked.removeAnt();
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

        else if (not is_playing && app_event.mouseButton.button == sf::Mouse::Right)
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
    if (is_playing)
    {
      // Add a sugar every 100 iterations.
      if (iteration % 100 == 0)
      {
        Place rand_sugar_place = grid.getRandEmptyPlace();

        if (rand_sugar_place.putSugar())
        {
          grid.setPlace(rand_sugar_place);
          std::cout << "A new pile of sugar appears." << std::endl;
        }
      }

      updateSetAnts(grid, ants);
      consistencyTest(grid, ants, "Simulation " + std::to_string(iteration));
      grid.decreasePheroSugar(3);
      grid_state = getGridState(grid, ants);
      evolution(grid, ants, set_nests, 20, 10);
      txt_pause.setString("Space to pause.");
    }
    else
      txt_pause.setString("Space to play.");

    // Draw all elements of the windows.
    window.draw(txt_pause);
    window.draw(txt_mouse);
    window.draw(txt_arrow);
    window.display();

    // Pause the simulation.
    sf::sleep(sf::milliseconds(time));
    iteration++;
  }
  return 0;
}
