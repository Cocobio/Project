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
		char Country[2];
		string City;
		string AccentCity;
		string Region;
		unsigned Population;
		float Latitude;
		float Longitude;
		pair<float,float> geopoint;
		
	public:
		// Constructor
		City_Struct(string country, string city, string accent_city, string region, size_t population, float latitude, float longitude, float geopoint_x, float geopoint_y) {
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
		typedef City_Struct		City;
		// typedef vector<City> 	Container;

	private:
		// Container cities;
		QuadTree<float, string> quadtree;
		
	public:
		WorldCityMap() : quadtree(0.0, 0.0, 360.0, 180.0) {}

		void add_city(City c, int i);
		void remove_city(City c);

		unsigned long long population_query_by_point(pair<float,float> point);
		unsigned long long population_query_by_point(float x, float y);
		unsigned long long population_query_by_region(pair<float,float> center, float width, float height);
};

#include "WorldCityMap.cpp"

#endif