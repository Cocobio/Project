/*
podemos hacerlo con flotantes en vez de enteros?
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "WorldCityMap.hpp"

// tmp
#include <cmath>
#include "QuadTree.hpp"

#define CITY_FILE "C:/worldcitiespop_fixed.csv"

using namespace std;

// Parser for csv lines
WorldCityMap::City parse_city_from_string(string line) {
	istringstream csvStream(line);

	string tmp;

	string country;
	string city;
	string accentCity;
	string region;
	unsigned population;
	float latitude;
	float longitude;
	float geopoint_x;
	float geopoint_y;

	// Read country, city, accent city and region
	getline(csvStream, country ,';');
	getline(csvStream, city ,';');
	getline(csvStream, accentCity ,';');
	getline(csvStream, region ,';');

	// Parsing population and geographic 
	csvStream >> population;
	getline(csvStream, tmp ,';');
	getline(csvStream, tmp ,';');
	getline(csvStream, tmp ,';');
	csvStream >> geopoint_x;
	getline(csvStream, tmp ,',');
	csvStream >> geopoint_y;

	// difference convention for denoting the '.'
	latitude = geopoint_x;
	longitude = geopoint_y;

	return WorldCityMap::City(country,city,accentCity,region,population,latitude,longitude,geopoint_x,geopoint_y);
}

int main() {
	cout << "Hello world!" << endl;

	WorldCityMap world_map;

	ifstream file;
	string tmp;

	file.open(CITY_FILE);
	getline(file, tmp);

	// Read data from the .csv
	// unsigned i=1;
	// while(getline(file, tmp))
	// 	world_map.add_city(parse_city_from_string(tmp));

	unsigned limite = -1;
	vector<WorldCityMap::City> all_cities;

	for(int i=0; getline(file, tmp) && i<limite; i++) {
		// cout << i << endl;
		// getline(file, tmp);
		WorldCityMap::City t = parse_city_from_string(tmp);
		// t.print();
		if (world_map.add_city(t))
			all_cities.push_back(t);
	}

	file.close();

	// region
	float x = 37.0, y = 40.5, w = 6.0, h = 2.0;	

	cout << endl << world_map.population_query_by_region(make_pair(x,y),w,h) << endl;

	unsigned long long p = 0;
	for (auto &c : all_cities) {
		if (fabs(x-c.Longitude)*2<=w && fabs(y-c.Latitude)*2<=h)
			p += c.Population;
	}

	cout << endl << p << endl;

	// int option, exit=5;
	// string menu = "Menu:\n1.-Search with geopoint.\n2.-Remove\n3.-Population by Region\n4.-Cities on Region\n5.-Exit";
	// float x, y, w, h;

	// cout << menu << endl;
	// cin >> option;
	// while(option!=exit) {
	// 	cin >> y >> x;
		
	// 	switch(option) {
	// 		case 1:
	// 		cout << world_map.population_query_by_point(x,y) << endl;
	// 		break;

	// 		case 2:
	// 		world_map.remove_city_by_geopoint(x,y);
	// 		break;

	// 		case 3:
	// 		cin >> w >> h;
	// 		cout << world_map.population_query_by_region(make_pair(x,y),w,h) << endl;
	// 		break;

	// 		case 4:
	// 		cin >> w >> h;
	// 		cout << world_map.n_cities_query_by_region(make_pair(x,y),w,h) << endl;
	// 		break;
	// 	}

	// 	cout << "new option: " ;
	// 	cin >> option;
	// }
	
	return 0;
}