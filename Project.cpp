/*
podemos hacerlo con flotantes en vez de enteros?
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <ctime>

#include "WorldCityMap.hpp"

// tmp
#include <cmath>
#include <algorithm>
#include <random>

#define CITY_FILE "C:/worldcitiespop_fixed.csv"

using namespace std;

template<class F>
double time_it(F func) {
	clock_t t = clock();
	func();
	t = clock() - t; 
	return ((double)t)/CLOCKS_PER_SEC;
}

// Parser for csv lines
WorldCityMap::City parse_city_from_string(string line) {
	istringstream csvStream(line);

	string tmp;

	string country;
	string city;
	string accentCity;
	string region;
	unsigned long population;
	double latitude;
	double longitude;
	double geopoint_x;
	double geopoint_y;

	// Read country, city, accent city and region
	getline(csvStream, country ,';');
	getline(csvStream, city ,';');
	getline(csvStream, accentCity ,';');
	getline(csvStream, region ,';');

	// Parsing population and geographic 
	csvStream >> population;
	// cout << city << " " << population << endl;
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
	clock_t t; 
	double time_taken; 

	cout << "Node size: " << sizeof(WorldCityMap::PRTree::QuadTreeNode) << endl;

	WorldCityMap world_map;

	ifstream file;
	string tmp;

	////////////////////////////
	// Read data from the .csv
	file.open(CITY_FILE);
	getline(file, tmp);

	vector<WorldCityMap::City> full_dataset;
	while(getline(file, tmp))
		full_dataset.push_back(parse_city_from_string(tmp));
	file.close();

	///////////////////////////
	// Shuffle
	auto rng = std::default_random_engine {};
	shuffle(begin(full_dataset), end(full_dataset), rng);

	full_dataset[0].print();

	vector<WorldCityMap::City> quadtree_cities;
	long long total_population = 0;
	for(auto &city : full_dataset) {
		total_population += city.Population;
		if (world_map.add_city(city))
			quadtree_cities.push_back(city);
	}


	cout << "El total total de habitantes es: " << total_population << endl;
	// cout << "Numero de nodos: " << world_map.quadtree_n_nodes() << endl;
	// cout << "Memoria usada por quadtree: " << world_map.sizeof_quadtree()/(1024*1024) << " MB." << endl;
	// cout << "Puntos almacenados: " << world_map.size() << " " << quadtree_cities.size() << endl;

	// for(auto &city : quadtree_cities) {
	// 	// cout << "delete: " << city.City << endl;
	// 	world_map.remove_city(city);
	// }

	map<size_t,size_t> depths = world_map.get_leaf_depths();

	for (auto &d : depths)
		cout << d.first << " : " << d.second << endl;



	// cout << world_map.n_cities_query_by_region(make_pair(0,0),360,180) << endl;

	// cout << quadtree_cities.size() << " cities added to the quadtree" << endl;

	// region
	double x = 0.0, y = 0., w = 360, h = 180;
	// cout << "searching!" << endl;
	// cout << "Poblacion total por region: " << world_map.population_query_by_region(make_pair(x,y), w, h) << endl;
	// cout << "Poblacion total por region iter: " << world_map.population_query_by_region_iter(make_pair(x,y), w, h) << endl;

	// x = 40, y = -70, w = 15, h = 20;
	time_taken = time_it([&world_map,&x,&y,&w,&h](){cout << "\nPoblacion total por region: " << world_map.population_query_by_region(make_pair(x,y), w, h) << endl;});
	cout << "time taken: " << time_taken << endl;
	time_taken = time_it([&world_map,&x,&y,&w,&h](){cout << "Poblacion total por region iter: " << world_map.population_query_by_region_iter(make_pair(x,y), w, h) << endl;});
	cout << "time taken: " << time_taken << endl;
	
	
	// cout << endl << world_map.population_query_by_region(make_pair(x,y),w,h) << endl;

	unsigned long long p = 0;
	for (auto &c : quadtree_cities) {
		if (fabs(x-c.Longitude)*2<=w && fabs(y-c.Latitude)*2<=h)
			p += c.Population;
	}

	cout << "Fuerza bruta por region: " << p << endl;

	total_population = 0;
	for (auto &c : quadtree_cities)
		total_population += c.Population;
	cout << "Todas poblacion de las ciudades insertadas: " << total_population << endl;

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


	// ******************************************************************************
	// 2D depth graph information
	// unsigned p_x = 360*30;
	// unsigned p_y = 180*30;

	// //// TIME MEASURE
	// t = clock();
	// vector<size_t> histogram2D = world_map.get_2D_depth_histogram(p_x,p_y);
	// t = clock() - t; 
	// time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	// /////////////////

	// cout << "size of histogram: " << histogram2D.size() << endl;

	// ofstream output_file("./Data/2d histogram data.txt");

	// // 2D histogram
	// for(auto &d : histogram2D)
	// 	output_file << d << ";";
	// output_file.close();
	// cout << "Tiempo en calcular el histograma: " << time_taken << endl; 
	// ******************************************************************************

	string balanced_parentheses = world_map.get_balanced_parentheses();

	ofstream balanced_parentheses_file("./Data/balanced_parentheses.txt");
	balanced_parentheses_file << balanced_parentheses << endl;
	balanced_parentheses_file.close();
	
	world_map.clear();
	cout << "Despues de borrar todo\nNodos: " << world_map.quadtree_n_nodes() << "\nPuntos: " << world_map.size() << endl;
	return 0;
}