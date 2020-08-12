#include "WorldCityMap.hpp"

void WorldCityMap::add_city(City c) {
	// verify if c was already added *********************
	// if (quadtree.insert(c))
	cities.push_back(c);
}