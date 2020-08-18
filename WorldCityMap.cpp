#include "WorldCityMap.hpp"

bool WorldCityMap::add_city(City c) {
	// City_Compact c_compact;
	// c_compact.Latitude = c.Latitude;
	// c_compact.Longitude = c.Longitude;
	// c_compact.Population = c.Population;

	// If not repeated inserted
	// if (quadtree.insert(c.Latitude, c.Longitude, cities.size()))
		// cities.push_back(c_compact);

	// cout << c.City << " " << c.Longitude << "\t" << c.Latitude << endl;

	// bool inserted = quadtree.insert(c.Longitude, c.Latitude, c.Population);

	// if (!inserted)
	// 	cout << "Not inserted!" << endl;
	return quadtree.insert(c.Longitude, c.Latitude, c.Population);
}

bool WorldCityMap::remove_city(City c) {
	return quadtree.remove(c.Longitude, c.Latitude);
}

void WorldCityMap::remove_city_by_geopoint(float x, float y) {
	quadtree.remove(x,y);
}

unsigned long long WorldCityMap::population_query_by_point(pair<float,float> point) {
	return quadtree.search_point(point.first, point.second);
}

unsigned long long WorldCityMap::population_query_by_point(float x, float y) {
	return quadtree.search_point(x,y);
}

unsigned long long WorldCityMap::population_query_by_region(pair<float,float> center, float width, float height) {
	unsigned long long population = 0;

	quadtree.search_region(center.first, center.second, width, height, [&population](point_id a) -> void { population += a; });

	return population;
}

unsigned WorldCityMap::n_cities_query_by_region(pair<float,float> center, float width, float height) {
	unsigned n_of_cities = 0;

	quadtree.search_region(center.first, center.second, width, height, [&n_of_cities](point_id a) -> void { n_of_cities++; });

	return n_of_cities;
}


