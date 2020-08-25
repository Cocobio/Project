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
	return ((double)t)/(CLOCKS_PER_SEC);
}

double save_image_histogram(size_t c, size_t r, WorldCityMap &map, ofstream &output_file);
void test_random_point_searches(WorldCityMap &world_map);
void test_contained_point_searches(WorldCityMap &world_map, vector<WorldCityMap::City> &cities);
void test_region_searches(WorldCityMap &world_map);

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
	// Testing variables
	size_t c = 360*30,	r = 180*30;
	size_t step = -1;
	size_t current = 1;
	ofstream output_file("Data/times.txt");

	////////////////////////////
	// Read data from the .csv
	file.open(CITY_FILE);
	getline(file, tmp);

	vector<WorldCityMap::City> full_dataset;
	vector<WorldCityMap::City> quadtree_cities;
	quadtree_cities.reserve(full_dataset.size());
	// int lim = 0;
	long long total_population = 0;
	
	while(getline(file, tmp)) {
		auto c_tmp = parse_city_from_string(tmp);
		
		total_population += c_tmp.Population;
		full_dataset.push_back(c_tmp);
		// if (lim++>1000) break;
		if (world_map.add_city(c_tmp))
			quadtree_cities.push_back(c_tmp);
	}
	file.close();
	world_map.clear();

	///////////////////////////
	// Shuffle
	auto rng = std::default_random_engine {std::random_device{}()};
	rng.seed(time(0));
	shuffle(begin(full_dataset), end(full_dataset), rng);

	full_dataset[0].print();
	full_dataset[1].print();

	// size_t inserted = 0;
			

	// ofstream file_depth_avr("Data/avr_depth.txt");
	double profundidad = 0.0;
	unsigned inserted = 0;
	int j = 1;

	cout << "quadtree_cities.size()/100.0 = " << quadtree_cities.size()/100.0 << endl;

	t = clock();
	for(int i=1; i<=quadtree_cities.size(); i++) {
		auto city = quadtree_cities[i-1];

		world_map.add_city(city);
		// profundidad += world_map.point_depth(city.Longitude, city.Latitude);
		// inserted++;

		// if ((i>=quadtree_cities.size()/100.0*j)) {
		// 	j++;
		// 	// t = clock() - t; 
		// 	// output_file << ((double)t)/(CLOCKS_PER_SEC) << ";";
		// 	// t=clock();

		// 	// file_depth_avr << quadtree_cities.size() << ";" << world_map.quadtree_n_nodes()-quadtree_cities.size() << ";" << world_map.white_node_size() << endl;;
		// 	// file_depth_avr << profundidad / inserted << ";";
		// 	inserted = 0;
		// 	profundidad = 0;
		// }
	// for(int i=1; i<=full_dataset.size(); i++) {
		// auto city = full_dataset[i-1];
		// total_population += city.Population;
		
		// if (world_map.add_city(city)) {
		// 	quadtree_cities.push_back(city);
		// 	profundidad += world_map.point_depth(city.Longitude, city.Latitude);
		// 	inserted++;
		// }

		// if ((i>=full_dataset.size()/100.0*j)) {
		// 	j++;
		// 	t = clock() - t; 
		// 	output_file << ((double)t)/(CLOCKS_PER_SEC) << ";";
		// 	t=clock();

		// 	// file_depth_avr << quadtree_cities.size() << ";" << world_map.quadtree_n_nodes()-quadtree_cities.size() << ";" << world_map.white_node_size() << endl;;
		// 	file_depth_avr << profundidad / inserted << ";";
		// 	inserted = 0;
		// 	profundidad = 0;
		// }
	}
	t = clock() - t; 
	cout << "Tiempo total para insertar: " << ((double)t)/(CLOCKS_PER_SEC) << ";";

	// file_depth_avr.close();


	// cout << "El total total de habitantes es: " << total_population << endl;
	cout << "Numero de nodos: " << world_map.quadtree_n_nodes() << endl;
	cout << "Memoria usada por quadtree: " << world_map.sizeof_quadtree()/(1024*1024) << " MB." << endl;
	cout << "Puntos almacenados: " << world_map.size() << " " << quadtree_cities.size() << endl;

	// for(auto &city : quadtree_cities) {
	// 	// cout << "delete: " << city.City << endl;
	// 	world_map.remove_city(city);
	// }

	map<size_t,size_t> depths;
	cout << "calculo de profundidades " << time_it([&depths, &world_map]() ->void { depths = world_map.get_leaf_depths(); }) << endl;

	// for (auto &d : depths)
	// 	cout << d.first << " : " << d.second << endl;


	// // ofstream nodes_depth_by_type_histogram("Data/histograma de profundidad de nodos por tipo.txt");

	// vector<map<size_t,size_t>> histograma_by_type = world_map.get_nodes_depths();
	// for (auto &d : histograma_by_type[0])
	// 	cout << d.first << " : " << d.second << endl;
	// for (auto &d : histograma_by_type[1])
	// 	cout << d.first << " : " << d.second << endl;
	// for (auto &d : histograma_by_type[2])
	// 	cout << d.first << " : " << d.second << endl;



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

	

	// ofstream histo2D("Data/2dHistogram.txt");
	// cout << "Tiempo en calcular el histograma 2d: " << save_image_histogram(c,r,world_map,histo2D) << endl;
	// histo2D.close();

	// string balanced_parentheses = world_map.get_balanced_parentheses();
	// cout << balanced_parentheses << endl;

	// ofstream balanced_parentheses_file("./Data/balanced_parentheses.txt");
	// balanced_parentheses_file << balanced_parentheses << endl;
	// balanced_parentheses_file.close();

	// test_random_point_searches(world_map);
	// test_contained_point_searches(world_map, quadtree_cities);
	// test_region_searches(world_map);
	
	output_file.close();
	// world_map.clear();
	// cout << "Despues de borrar todo\nNodos: " << world_map.quadtree_n_nodes() << "\nPuntos: " << world_map.size() << endl;
	j=1;
	ofstream file_remove_depth_avr("Data/remove t y depth avr.txt");

	// shuffle cities to be removed
	rng = std::default_random_engine {std::random_device{}()};
	rng.seed(time(0));
	shuffle(begin(quadtree_cities), end(quadtree_cities), rng);

	cout << quadtree_cities[0].City << endl;
	profundidad = 0;

	cout << "about to remove!" << endl;
	t = clock();
	for(int i=1; i<=quadtree_cities.size(); i++) {
		// cout << i << endl;
		auto city = quadtree_cities[i-1];

		profundidad += world_map.point_depth(city.Longitude, city.Latitude);
		world_map.remove_city(city);

		if (i>=quadtree_cities.size()/100.1*j) {
			j++;
			t = clock() - t; 
			file_remove_depth_avr << ((double)t)/(CLOCKS_PER_SEC) << ";";
			t=clock();

			file_remove_depth_avr << profundidad/(quadtree_cities.size()/100.0) << endl;
			profundidad = 0;
		}
	}

	file_remove_depth_avr.close();

	cout << "done!" << endl;
	full_dataset.clear();
	quadtree_cities.clear();

	cout << "everything free" << endl;
	return 0;
}

double save_image_histogram(size_t c, size_t r, WorldCityMap &map, ofstream &output_file) {
	// ******************************************************************************
	// 2D depth graph information
	vector<size_t> histogram2D;
	double time_taken = time_it([&histogram2D, &c, &r, &map]() { histogram2D = map.get_2D_depth_histogram(c,r); });

	// Write on the file
	for(auto &d : histogram2D)
		output_file << d << ";";
	output_file << endl;
	return time_taken;
	// ******************************************************************************
}

void test_random_point_searches(WorldCityMap &world_map) {
	ofstream file_output("Data/random point search.txt");

	unsigned bins = 20000;
	unsigned data = bins*100;
	double profundidad = 0;

	int j=1;

	clock_t t = clock();
	for(int i=0; i<=data; i++) {
		double x = ((double)rand() / RAND_MAX)*360-180, y = ((double)rand() / RAND_MAX)*180-90;
		world_map.population_query_by_point(x,y);
		profundidad += world_map.point_depth(x,y);

		if (i>=data/100*j) {
			j++;

			t = clock() - t;
			file_output << ((double)t)/(CLOCKS_PER_SEC) << ";";
			t = clock();

			file_output << profundidad/(data/100.0) << endl;
			profundidad = 0;
		}
	}

	file_output.close();
}


void test_contained_point_searches(WorldCityMap &world_map, vector<WorldCityMap::City> &cities) {
	ofstream file_output("Data/contained point search.txt");

	unsigned bins = 20000;
	unsigned data = bins*100;
	double profundidad = 0;

	int j=1;

	clock_t t = clock();
	for(int i=0; i<=data; i++) {
		auto city = cities[rand()%cities.size()];
		world_map.population_query_by_point(city.Longitude,city.Latitude);
		profundidad += world_map.point_depth(city.Longitude,city.Latitude);

		if (i>=data/100*j) {
			j++;

			t = clock() - t;
			file_output << ((double)t)/(CLOCKS_PER_SEC) << ";";
			t = clock();

			file_output << profundidad/(data/100.0) << endl;
			profundidad = 0;
		}
	}

	file_output.close();
}


void test_region_searches(WorldCityMap &world_map) {
	ofstream file_output("Data/random region search.txt");

	unsigned test = 20;

	int j=1;

	for (int d=10; d<=140; d++) {
		cout << d << endl;
		unsigned long long n_of_cities = 0;

		clock_t t = clock();
		for(int i=0; i<=test; i++) {
			double x = ((double)rand() / RAND_MAX)*(360-d)-180+d/2, y = ((double)rand() / RAND_MAX)*(180-d)-90+d/2;
			n_of_cities += world_map.n_cities_query_by_region(make_pair(x,y), d, d);
		}
		t = clock() - t;

		file_output << ((double)t)/(CLOCKS_PER_SEC) << ";" << n_of_cities/float(test) << endl;
	}

	file_output.close();
}


void test_single_batch_of_point_searches(WorldCityMap &world_map, vector<WorldCityMap::City> &cities, ofstream &file_output) {
	// ofstream file_output("Data/random point search.txt");

	unsigned bins = 20000;
	unsigned data = bins*100;
	double profundidad = 0;

	int j=1;

	clock_t t = clock();
	for(int i=0; i<=data; i++) {
		double x = ((double)rand() / RAND_MAX)*360-180, y = ((double)rand() / RAND_MAX)*180-90;
		world_map.population_query_by_point(x,y);
		profundidad += world_map.point_depth(x,y);

		if (i>=data/100*j) {
			j++;

			t = clock();

			profundidad = 0;
		}
	}
	t = clock() - t;
	file_output << ((double)t)/(CLOCKS_PER_SEC) << ";";
	file_output << profundidad/data << endl;

	profundidad = 0;

	j=1;

	t = clock();
	for(int i=0; i<=data; i++) {
		auto city = cities[rand()%cities.size()];
		world_map.population_query_by_point(city.Longitude,city.Latitude);
		profundidad += world_map.point_depth(city.Longitude,city.Latitude);

		if (i>=data/100*j) {
			j++;

			t = clock() - t;
			file_output << ((double)t)/(CLOCKS_PER_SEC) << ";";
			t = clock();

			file_output << profundidad/(data/100.0) << endl;
			profundidad = 0;
		}
	}

	file_output.close();
}

