/*
WorldCityMap

Definition of a City class that contains all the data from the dataset

Implementation of the interface between the quadtree and the requirements for the project assignment.
*/

#ifndef WORLD_CITY_MAP
#define WORLD_CITY_MAP

#include <vector>
#include <map>
#include <string>
#include <utility>
#include "QuadTree.hpp"

using namespace std;

// Playing with precisions float/double
typedef double precision;

// Struct for saving the information of a single city
class City_Struct {
	public:
		typedef precision		value_t;

		char Country[2];
		string City;
		string AccentCity;
		string Region;
		unsigned long Population;
		value_t Latitude;
		value_t Longitude;
		pair<value_t,value_t> geopoint;
		
	public:
		// Constructor
		City_Struct(string country, string city, string accent_city, string region, unsigned long population, value_t latitude, value_t longitude, value_t geopoint_x, value_t geopoint_y) {
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

// Class for organizing
class WorldCityMap {
	public:
		typedef City_Struct						City;
		// typedef vector<City> 					Container;
		typedef unsigned 						point_id;
		typedef precision						value_t;
		typedef QuadTree<value_t, point_id> 	PRTree;

	private:
		// Container cities;
		PRTree quadtree;
		
	public:
		WorldCityMap() : quadtree(0.0, 0.0, 360.0, 180.0) {}
		~WorldCityMap() { quadtree.clear(); }

		// Add and remove functions
		bool add_city(City c);
		bool remove_city(City c);
		void remove_city_by_geopoint(value_t x, value_t y);

		// Queries for population and number of cities on point and region
		unsigned long long population_query_by_point(pair<value_t,value_t> point);
		unsigned long long population_query_by_point(value_t x, value_t y);
		unsigned long long population_query_by_region(pair<value_t,value_t> center, value_t width, value_t height);
		unsigned n_cities_query_by_region(pair<value_t,value_t> center, value_t width, value_t height);

		// Size, clean and size of the quadtree
		size_t sizeof_quadtree();
		unsigned quadtree_n_nodes();
		size_t size();
		void clear();

		// Get a vector for the depth on the black nodes
		map<size_t,size_t> get_leaf_depths();

		// Get a matrix with the maximum depth by region
		vector<size_t> get_2D_depth_histogram(size_t column, size_t row);
};

#include "WorldCityMap.cpp"

#endif