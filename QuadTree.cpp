#include "QuadTree.hpp"

template<class T, class K>
typename QuadTree<T,K>::QuadTreeNode* QuadTree<T,K>::new_node(typename QuadTreeNode::NodeType t) {
	QuadTreeNode* n = new QuadTreeNode;
	n->type = t;
	
	for(auto &child : n->children)
		child = nullptr;

	return n;
}

template<class T, class K>
void QuadTree<T,K>::returnavail(QuadTreeNode* p) {
	free(p);
}

// Identify the quadrant
template<class T, class K>
typename QuadTree<T,K>::Quadrant QuadTree<T,K>::compare(QuadTreeNode *p, value_t x, value_t y) {
	if (p->x<x) {
		if (p->y<y)
			return SW;
		else
			return NW;
	}
	else if (p->y<y)
		return SE;
	else
		return NE;
}

// Insert point drive
template<class T, class K>
bool QuadTree<T,K>::insert(value_t p_x, value_t p_y, point_id p_id) {
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = p_x;
	p->y = p_y;
	p->info = p_id;

	if (insert(p,center_x,center_y,Lx,Ly))
		return true;
	
	returnavail(p);
	return false;
}

// Insert point on tree
// returns whether the point was inserted or not
template<class T, class K>
bool QuadTree<T,K>::insert(QuadTreeNode *p, value_t x, value_t y, value_t lx, value_t ly) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	// If there is no node root
	if (root==nullptr) {
		root = p;
		return true;
	}
	
	//
	QuadTreeNode *u, *t;
	Quadrant q;

	// If the node root is a leaf 
	if(root->type != QuadTreeNode::GREY) {
		if (p->x==root->x && p->y==root->y) return false;
		else {
			u = root;
			root = new_node(QuadTreeNode::GREY);
			q = compare(u, x, y);
			root->children[q] = u;
			// cout << "root Q: " << q << endl;
		}
	}
	// 
	t = root;
	q = compare(p,x,y);
	// cout << "pointQ: " << q << endl;

	while(t->children[q]!=nullptr && t->children[q]->type == QuadTreeNode::GREY) {
		// cout << "first while" << endl;
		t = t->children[q];
		x += XF[q]*lx;
		y += YF[q]*ly;
		lx /= 2.0;
		ly /= 2.0;
		q = compare(p,x,y);
	}

	if (t->children[q]==nullptr) t->children[q] = p;
	else if (fabs(t->children[q]->x-p->x)<0.00001 && fabs(t->children[q]->y-p->y)<0.00001) return false;
	else {
		u = t->children[q];
		Quadrant uq;

		do {
			t->children[q] = new_node(QuadTreeNode::GREY);
			t = t->children[q];

			x += XF[q]*lx;
			y += YF[q]*ly;
			lx /= 2.0;
			ly /= 2.0;
			q = compare(p,x,y);
			uq = compare(u,x,y);

			if (lx <= 0.000001 || ly <= 0.000001) {
			cout << "===============\nPoints: ";
			cout << p->x << ", " << p->y << "\tu:" << u->x << ", " << u->y << endl;
			cout << p->info << "\t" << u->info << endl;
			cout << "===============\nQuadrant: " << x << ", " << y << ":\t";
			cout << lx << "  " << ly << "\n q:" << q << "   uq:" << uq << endl << endl;
				cout << "lx and ly to small!" << endl;
				throw;
			}

		} while (q==uq);

		t->children[q] = p;
		t->children[uq] = u;
	}

	return true;
}

// Drive function
template<class T, class K>
typename QuadTree<T,K>::point_id QuadTree<T,K>::search_point(value_t x, value_t y) {
	return search_point(x,y,center_x,center_y,Lx,Ly);
}

// Drive function
template<class T, class K> template<class R>
void QuadTree<T,K>::search_region(value_t x, value_t y, value_t d_x, value_t d_y, R report) {

}





/*
ga;nzafieng;Nzafieng;06;414856;0,0666667;11,8833333;0.0666667,11.8833333
*/