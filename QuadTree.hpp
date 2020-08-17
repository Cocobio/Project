/*
QuadTree

	WE ASSUME THAT ALL DATA POINTS ARE UNIQUE, OTHER WISE AN OVERFLOW LIST WOULD BE REQUIRED ON 

	Members:

		children: An array of 4 pointers to the nodes representing the quadrants
						  |
					   0  |  1
					      |
					-------------
						  |
					   2  |  3
						  |

references: 
	https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/PRquadtree.html
	http://www.shuizilong.com/house/wp-content/uploads/2012/02/The-Design-and-Analysis-of-Spatial-Data-StructuresHananSamet.pdf		pag 92
*/

#ifndef QUAD_TREE
#define QUAD_TREE

#include <vector>
#include <cmath>

// tmp
#include <iostream>
#include <stdexcept>

using namespace std;

template <class T, class K>
class QuadTree {
	public:
		typedef T 					value_t;
		typedef K					point_id;

		//			   00, 01, 10, 11
		enum Quadrant {NW, NE, SW, SE};

	// private:
		struct QuadTreeNode {
				enum NodeType {WHITE, GREY, BLACK};

				QuadTreeNode *children[4];
				value_t x, y;
				point_id info;
				NodeType type;
		};

	private:
		QuadTreeNode* root;
		// vector<QuadTreeNode> node_pool;

		double Lx, Ly;
		value_t center_x, center_y;

		// memory managment
		QuadTreeNode* new_node(typename QuadTreeNode::NodeType t);
		void returnavail(QuadTreeNode* p);

		Quadrant compare(QuadTreeNode *p, value_t x, value_t y);
		bool insert(QuadTreeNode *p, value_t x, value_t y, double lx, double ly);
		point_id search_point(QuadTreeNode *p, value_t x, value_t y, double lx, double ly);

	public:
		// Constructor
		QuadTree(value_t x, value_t y, double lx, double ly) : center_x(x), center_y(y), Lx(lx), Ly(ly), root(nullptr) {}

		// Destructor
		~QuadTree() {

		}

		bool insert(value_t p_x, value_t p_y, point_id p_id);
		
		point_id search_point(value_t x, value_t y);

		template <class R>
		void search_region(value_t x, value_t y, value_t d_x, value_t d_y, R report);
};

#include "QuadTree.cpp"

#endif
