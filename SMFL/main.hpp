#ifndef MAIN_HPP
#define MAIN_HPP

#include "Grid.hpp"
#include "GridAnts.hpp"

void consistencyTest(Grid &grid, GridAnts &ants, std::string title);
std::vector<std::vector<int>> getGridState(Grid &grid, GridAnts &ants);

#endif
