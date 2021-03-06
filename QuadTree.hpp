/*
QuadTree

	We dont allow repited points, implemented using a EPSILON. If the difference of 2 points on both axis 
	are less than an EPSILON, we discard the point.

	Node struct:
		children: An array of 4 pointers to the nodes representing the quadrants
						  |
					   0  |  1
					      |
					-------------
						  |
					   2  |  3
						  |
		x, y: Coordinates of the point saved on the region
		info: Aditional info about the point saved
		type: identifier for type of node {WHITE is a placeholder, GREY are non-leaf nodes, BLACK are leaf nodes}

	QuadTree Members:
		root: The root of the tree
		Lx, Ly: The size of the region used on the quadtree. 
		center_x, center_y: The center of the region with width Lx and height Ly.

		node_count: Tmp member to count the amount of nodes being used. (adds when new node are created and decreases when nodes are deleted)

references: 
	https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/PRquadtree.html
	http://www.shuizilong.com/house/wp-content/uploads/2012/02/The-Design-and-Analysis-of-Spatial-Data-StructuresHananSamet.pdf		pag 92
*/

#ifndef QUAD_TREE
#define QUAD_TREE

// #define EPSILON 0.00001

// #include <vector>
#include <cmath>
#include <queue>
#include <functional>

// Only for balanced parentheses function
#include <string>

using namespace std;

template <class T, class K>
class QuadTree {
	public:
		typedef T 					value_t;
		typedef K					point_id;
		typedef double				precision;

		//			   			 00, 01, 10, 11
		enum Quadrant : uint8_t {NW, NE, SW, SE};

	// private:
		struct QuadTreeNode {
				enum NodeType : uint8_t {WHITE, GREY, BLACK};

				QuadTreeNode *children[4];
				value_t x, y;
				point_id info;
				NodeType type;
		};

	private:
		QuadTreeNode* root;
		// vector<QuadTreeNode> node_pool;

		precision Lx, Ly;
		precision center_x, center_y;

		size_t _size;

		// For testing delete and counting the number of nodes
		unsigned node_count;

		// memory managment
		QuadTreeNode* new_node(typename QuadTreeNode::NodeType t);
		void returnavail(QuadTreeNode* p);

		// Identify quadrant in which p lays using x and y as center
		Quadrant compare(QuadTreeNode *p, precision x, precision y);

		// Insert and remove functions
		bool insert(QuadTreeNode *p, precision x, precision y, precision lx, precision ly);
		bool remove(QuadTreeNode *p, precision x, precision y, precision lx, precision ly);

		// Search functions
		void search_point(QuadTreeNode *p, precision x, precision y, precision lx, precision ly, function<void(point_id&)> report);
		void search_region(QuadTreeNode *n, precision x, precision y, precision lx, precision ly, 
							value_t &r_x, value_t &r_y, value_t &lr_x, value_t &lr_y, 
							function<void(point_id&)> report);
		void bfs_by_region_iter(QuadTreeNode *n, precision x, precision y, precision lx, precision ly, 
							value_t &r_x, value_t &r_y, value_t &lr_x, value_t &lr_y, 
							function<void(QuadTreeNode*&, size_t&, pair<precision,precision>&, precision&, precision&)> report);

		// recursive balanced parentheses
		void balanced_parentheses(QuadTreeNode *r, string& representation);
		
	public:
		// Constructor
		QuadTree(value_t x, value_t y, precision lx, precision ly) : center_x(x), center_y(y), Lx(lx), Ly(ly), root(nullptr), node_count(0), _size(0) {}

		// Destructor
		~QuadTree() { clear(); cout << "a;skjdf";}

		// Driver functions for insertion and delition of nodes
		bool insert(value_t x, value_t y, point_id p_id);
		bool remove(value_t x, value_t y);
		
		// Driver functions for search using point and region
		void search_point(value_t x, value_t y, function<void(point_id&)> report);
		void search_region(value_t x, value_t y, value_t d_x, value_t d_y, function<void(point_id&)> report);
		void bfs_by_region_iter(value_t x, value_t y, value_t d_x, value_t d_y, function<void(QuadTreeNode*&, size_t&, pair<precision,precision>&, precision&, precision&)> report);
		

		// For getting the number of nodes
		unsigned counter() {return node_count;}

		// Clean and return the number of points on the structure
		void clear();
		size_t size();

		// BFS function using a reported to work on the nodes.
		// The function should take the node and the level of the node on the tree
		void bfs(function<void(QuadTreeNode*&, size_t&, pair<precision,precision>&, precision&, precision&)> report);

		// Representation of balanced parentheses
		string balanced_parentheses();

		// Experimental
		void align_memory();

		// Only for testing
		unsigned point_depth(value_t x, value_t y);
		unsigned point_depth(QuadTreeNode *p, precision x, precision y, precision lx, precision ly);
		unsigned white_node_size();
};

#include "QuadTree.cpp"

#endif
