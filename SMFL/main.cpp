#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
// #include "coord.hpp"
// #include "place.hpp"
// #include "grille.hpp"
// #include "grilleFourmis.hpp"
// #include <fstream>
// #include <sstream>
// #include <iostream>
// #include <vector>
#include <SFML/Graphics.hpp>

int main(int argc, const char **argv)
{
  // Infrastructure docstest.
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    window.draw(shape);
    window.display();
  }

  return 0;
}