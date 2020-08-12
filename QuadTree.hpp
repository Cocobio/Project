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
					   3  |  2
						  |

references: 
	https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/PRquadtree.html
	http://www.shuizilong.com/house/wp-content/uploads/2012/02/The-Design-and-Analysis-of-Spatial-Data-StructuresHananSamet.pdf		pag 92
*/

#ifndef QUAD_TREE
#define QUAD_TREE


// template <class T>
// class Region {
// 	public:
// 		typedef T 		value_t;
// 	private:
// 		std::pair<value_t,value_t> x;
// 		std::pair<value_t,value_t> y;

// 	public:
// 		Region(value_t x_min, value_t x_max, value_t y_min, value_t y_max) {
// 			x = std::make_pair(x_min,x_max);
// 			y = std::make_pair(y_min,y_max);
// 		}

// 		Region(std::pair<value_t,value_t> x, std::pair<value_t,value_t> y) {
// 			this->x = x;
// 			this->y = y;
// 		}

// 		value_t const getX() { return x; };
// 		value_t const getY() { return y; };
// };

template <class T, class K>
class QuadTree {
	private:
		template <class C, class I>
		class QuadTreeNode {
			private:
				static enum NodeType = {WHITE, GREY, BLACK};

				QuadTreeNode<C,I> children[4];
				short x, y;
				I info;
				NodeType type;
				// Region region;

				void divide();

			public:
				// Constructor
				QuadTreeNode() {

				}

				// Destructor
				~QuadTreeNode() {

				}

				void add(std::pair<C,C> point);
				void remove(std::pair<C,C> point);
		};

	public:
		typedef T                                   value_t;
		typedef K                                   point_id;
		typedef QuadTreeNode<value_t, point_id>     Node;

	private:
		Node root;
		Node* node_pool;

	public:
		// Constructor
		QuadTree(std::pair<value_t,value_t> x, std::pair<value_t,value_t> y) {

		}

		// Destructor
		~QuadTree() {

		}
};

#include "QuadTree.cpp"

#endif
