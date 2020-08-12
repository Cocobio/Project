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

#define CITY_FILE "Data/worldcitiespop_fixed.csv"

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
	getline(csvStream, tmp ,';');
	population = stoul(tmp);
	getline(csvStream, tmp ,';');
	latitude = stof(tmp);
	getline(csvStream, tmp ,';');
	getline(csvStream, tmp ,',');
	geopoint_x = stof(tmp);
	getline(csvStream, tmp);
	geopoint_y = stof(tmp);

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
	longitude = stof(tmp);

	file.open(CITY_FILE);
	getline(file, tmp);

	// Read data from the .csv
	unsigned i=0;
	while(getline(file, tmp) && tmp.size()!=0) {
		world_map.add_city(parse_city_from_string(tmp));
		cout << i++ << endl;
	}

	getline(file,tmp);
	WorldCityMap::City test = parse_city_from_string(tmp);
	test.print();

	file.close();

	
	// cout << CITY_FILE << endl;
	
	return 0;
}