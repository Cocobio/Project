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
	while(getline(file, tmp))
		world_map.add_city(parse_city_from_string(tmp));

	// int limite = 100;

	// for(int i=0; getline(file, tmp) && i<limite; i++) {
	// 	// cout << i << endl;
	// 	// getline(file, tmp);
	// 	WorldCityMap::City t = parse_city_from_string(tmp);
	// 	world_map.add_city(t);
	// }

	file.close();

	int option;
	string menu = "Menu:\n1.-Search with geopoint.\n2.-Exit";
	float x, y;

	cout << menu << endl;
	cin >> option;
	while(option==1) {
		cin >> y >> x;
		cout << world_map.population_query_by_point(x,y) << endl;

		cout << "new option: " ;
		cin >> option;
	}
	
	return 0;
}