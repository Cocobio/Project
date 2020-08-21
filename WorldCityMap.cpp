#include "WorldCityMap.hpp"

// Add city to the quadtree and to a container if we set it up
bool WorldCityMap::add_city(City c) {
	// if (quadtree.insert(c.Longitude, c.Latitude, c.Population))
		// cities.push_back(c);

	return quadtree.insert(c.Longitude, c.Latitude, c.Population);
}

// Removes the city
bool WorldCityMap::remove_city(City c) {
	return quadtree.remove(c.Longitude, c.Latitude);
}

// Remove using latitude and longitude
void WorldCityMap::remove_city_by_geopoint(value_t x, value_t y) {
	quadtree.remove(x,y);
}

// Query the population on a single point
unsigned long long WorldCityMap::population_query_by_point(pair<value_t,value_t> point) {
	unsigned long long population = 0;

	// Creates a lambda function to set the population found on a point
	quadtree.search_point(point.first, point.second, [&population] (point_id &a) -> void { population = a; });
	
	return population; 
}

// same as above but not using a pair<>
unsigned long long WorldCityMap::population_query_by_point(value_t x, value_t y) {
	unsigned long long population = 0;

	quadtree.search_point(x,y, [&population] (point_id &a) -> void { population = a; });

	return population;
}

// Query population inside a region
unsigned long long WorldCityMap::population_query_by_region(pair<value_t,value_t> center, value_t width, value_t height) {
	unsigned long long population = 0;

	// searches on the quadtree using a lambda function that accumulates the population of the cities found
	quadtree.search_region(center.first, center.second, width, height, [&population](point_id &a) -> void { population += a; });

	return population;
}

// Query the number of cities found inside a region
unsigned WorldCityMap::n_cities_query_by_region(pair<value_t,value_t> center, value_t width, value_t height) {
	unsigned n_of_cities = 0;

	// Searches on the quadtree using a lambda function that counts the number of cities that are found inside the region
	quadtree.search_region(center.first, center.second, width, height, [&n_of_cities](point_id &a) -> void { n_of_cities++; });

	return n_of_cities;
}

// Return the number of bytes used by the quadtree
size_t WorldCityMap::sizeof_quadtree() {
	return sizeof(quadtree) + quadtree.counter()*sizeof(PRTree::QuadTreeNode);
}

// The total number of nodes on the quadtree
unsigned WorldCityMap::quadtree_n_nodes() {
	return quadtree.counter(); 
}

// Return the number of cities on the quadtree
size_t WorldCityMap::size() {
	return quadtree.size(); 
}

// Clear all the data
void WorldCityMap::clear() {
	quadtree.clear();
}

// BFS for getting a histogram on the depth of the leaf nodes
map<size_t,size_t> WorldCityMap::get_leaf_depths() {
	map<size_t,size_t> depths;

	quadtree.bfs([&depths] (PRTree::QuadTreeNode *a, size_t &d, pair<double,double> &center, double &lx, double &ly) -> void { if(a->type==PRTree::QuadTreeNode::BLACK) depths[d]++; } );

	return depths;
}

// BFS for getting maximum depth of a region
// Combine with a matrix for getting a beautiful 2D histogram
vector<size_t> WorldCityMap::get_2D_depth_histogram(size_t column, size_t row) {
	vector<size_t> histogram_2d(column*row,0);

	double d_x = 360.0/column, d_y = 180.0/row;


	for(int i=0; i<column; i++)
		for(int j=0; j<row; j++) {
			size_t *region_depth = &histogram_2d[i*row+j];
			auto func = [region_depth] (PRTree::QuadTreeNode* a, size_t& d, pair<double,double>& c, double& e, double& f) -> void { if (*region_depth<d) *region_depth = d; };
			quadtree.bfs_by_region_iter(-180+d_x/2.0+d_x*i,90-d_y/2.0-d_y*j,d_x-0.00001,d_y-0.00001, func);
			// cout << "matrix[" << i << "," << j << "]  " << *region_depth << "  center: " << -180+d_x/2.0+d_x*i << ", " << 90-d_y/2.0-d_y*j << "\t lx,ly: " << d_x << ", " << d_y << endl;
		}

	return histogram_2d;
}

string WorldCityMap::get_balanced_parentheses() {
	return quadtree.balanced_parentheses();
}


