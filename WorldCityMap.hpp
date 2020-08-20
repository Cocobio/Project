/*
*/

#ifndef WORLD_CITY_MAP
#define WORLD_CITY_MAP

#include <vector>
#include <string>
#include <utility>
#include "QuadTree.hpp"

// tmp
#include <iostream>

using namespace std;

// Struct for saving the information of a single city
class City_Struct {
	public:
		typedef double 			value_t;
		char Country[2];
		string City;
		string AccentCity;
		string Region;
		unsigned Population;
		value_t Latitude;
		value_t Longitude;
		pair<value_t,value_t> geopoint;
		
	public:
		// Constructor
		City_Struct(string country, string city, string accent_city, string region, size_t population, value_t latitude, value_t longitude, value_t geopoint_x, value_t geopoint_y) {
			Country[0] = country[0];
			Country[1] = country[1];
			City = city;
			AccentCity = accent_city;
			Region = region;
			Population = population;
			Latitude = latitude;
			Longitude = longitude;
			geopoint = make_pair(geopoint_x, geopoint_y);
		}

		// Destructor
		~City_Struct() {

		}

		void print() {
			cout << Country[0] << Country[1] << " " << City << " " << AccentCity << " " << Region << " " << Population << " " << Latitude << " " << Longitude << " (" << geopoint.first << ", " << geopoint.second << ")" << endl;
		}
};

// struct City_Compact {
// 	float Latitude;
// 	float Longitude;
// 	unsigned Population;
// };

// Class for organizing
class WorldCityMap {
	public:
		typedef City_Struct						City;
		// typedef vector<City> 					Container;
		typedef unsigned 						point_id;
		typedef double 							value_t;
		typedef QuadTree<value_t, point_id> 	PRTree;

	private:
		// Container cities;
		PRTree quadtree;
		
	public:
		WorldCityMap() : quadtree(0.0, 0.0, 360.0, 180.0) {}

		bool add_city(City c);
		bool remove_city(City c);
		void remove_city_by_geopoint(value_t x, value_t y);

		unsigned long long population_query_by_point(pair<value_t,value_t> point);
		unsigned long long population_query_by_point(value_t x, value_t y);
		unsigned long long population_query_by_region(pair<value_t,value_t> center, value_t width, value_t height);
		unsigned n_cities_query_by_region(pair<value_t,value_t> center, value_t width, value_t height);

		unsigned counter() { return quadtree.counter(); }
		size_t size() { return quadtree.size(); }
};

#include "WorldCityMap.cpp"

#endif