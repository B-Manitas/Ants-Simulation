#define DOCTEST_CONFIG_IMPLEMENT
#include "Action.hpp"
#include "Ant.hpp"
#include "Coord.hpp"
#include "Grid.hpp"
#include "GridAnts.hpp"
#include "Place.hpp"
#include "Doctest.h"

int main(int argc, const char **argv)
{
  doctest::Context context(argc, argv);
  int test_result = context.run();
  if (context.shouldExit())
    return test_result;

  return 0;
}
