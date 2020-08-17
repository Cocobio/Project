#include "WorldCityMap.hpp"

void WorldCityMap::add_city(City_Struct c, int i) {
	// City_Compact c_compact;
	// c_compact.Latitude = c.Latitude;
	// c_compact.Longitude = c.Longitude;
	// c_compact.Population = c.Population;

	// If not repeated inserted
	// if (quadtree.insert(c.Latitude, c.Longitude, cities.size()))
		// cities.push_back(c_compact);

	// cout << c.City << " " << c.Longitude << "\t" << c.Latitude << endl;

	bool inserted = quadtree.insert(c.Longitude, c.Latitude, c.City+" "+to_string(c.Population));

	// if (!inserted)
	// 	cout << "Not inserted!" << endl;
}

unsigned long long WorldCityMap::population_query_by_point(float x, float y) {
	cout << quadtree.search_point(x,y) << endl;
	return 0;
}