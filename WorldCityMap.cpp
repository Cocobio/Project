#include "WorldCityMap.hpp"

void WorldCityMap::add_city(City_Struct c) {
	// City_Compact c_compact;
	// c_compact.Latitude = c.Latitude;
	// c_compact.Longitude = c.Longitude;
	// c_compact.Population = c.Population;

	// If not repeated inserted
	// if (quadtree.insert(c.Latitude, c.Longitude, cities.size()))
		// cities.push_back(c_compact);

	// cout << c.City << " " << c.Longitude << "\t" << c.Latitude << endl;

	bool inserted = quadtree.insert(c.Longitude, c.Latitude, c.Population);

	// if (!inserted)
	// 	cout << "Not inserted!" << endl;
}

unsigned long long WorldCityMap::population_query_by_point(float x, float y) {
	return quadtree.search_point(x,y);
}

