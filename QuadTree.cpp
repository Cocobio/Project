#include "QuadTree.hpp"

template<class T, class K>
typename QuadTree<T,K>::QuadTreeNode* QuadTree<T,K>::new_node(typename QuadTreeNode::NodeType t) {
	QuadTreeNode* n = new QuadTreeNode;
	n->type = t;
	
	for(int i=0; i<4; i++)
		n->children[i] = nullptr;

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
bool QuadTree<T,K>::insert(value_t x, value_t y, point_id p_id) {
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = x;
	p->y = y;
	p->info = p_id;

	if (insert(p,center_x,center_y,Lx,Ly))
		return true;
	
	returnavail(p);
	return false;
}

// Insert point on tree
// returns whether the point was inserted or not
template<class T, class K>
bool QuadTree<T,K>::insert(QuadTreeNode *p, value_t x, value_t y, double lx, double ly) {
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
		}
	}
	// 
	t = root;
	q = compare(p,x,y);

	// Find the next quadrant
	while(t->children[q]!=nullptr && t->children[q]->type == QuadTreeNode::GREY) {
		t = t->children[q];
		x += XF[q]*lx;
		y += YF[q]*ly;
		lx /= 2.0;
		ly /= 2.0;
		q = compare(p,x,y);
	}

	// insert in quadrant
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

		} while (q==uq);

		t->children[q] = p;
		t->children[uq] = u;
	}

	return true;
}

// Drive functions
template<class T, class K>
void QuadTree<T,K>::remove(value_t x, value_t y) {
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = x;
	p->y = y;

	remove(p,center_x,center_y,Lx,Ly);
	returnavail(p);
}

// Delete node from tree
template<class T, class K>
void QuadTree<T,K>::remove(QuadTreeNode *p, value_t x, value_t y, double lx, double ly) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	if (root==nullptr) return;
	else if (root->type == QuadTreeNode::BLACK) {
		if (fabs(root->x-p->x)<0.00001 && fabs(root->y-p->y)<0.00001) {
			returnavail(root);
			root = nullptr;
		}	
		return;
	}

	QuadTreeNode *t = root, *f = nullptr, *ft, *tmp;
	Quadrant q, qf;

	do {
		q = compare(p,x,y);

		if (t->children[q]->type == QuadTreeNode::GREY && (t->children[q^1]!=nullptr ||
			t->children[q^2]!=nullptr || t->children[q^3]!=nullptr)) {
			f = t;
			qf = q;
		}
		
		ft = t;
		t = t->children[q];

		x += XF[q]*lx;
		y += YF[q]*ly;
		lx /= 2.0;
		ly /= 2.0;
	} while (t!=nullptr && t->type==QuadTreeNode::GREY);

	if (t==nullptr || fabs(t->x-p->x)>=0.00001 || fabs(t->y-p->y)>=0.00001)
		return;

	returnavail(t);
	ft->children[q] = nullptr;
	unsigned s = 0;

	for (int i=0; i<4; i++) {
		if (ft->children[i]!=nullptr) {
			if (ft->children[i]->type == QuadTreeNode::GREY) return;
		}
		else s++;
	}

	if (s>1) return;

	t = f==nullptr ? root : f->children[qf];
	for (int i = NW; t->children[i]==nullptr; i++) {
		q = static_cast<Quadrant>(i);
	}

	tmp = t->children[q];
	t->children[q] = nullptr;
	returnavail(t);
	t = tmp;

	if (f==nullptr) root = t;
	else f->children[qf] = t;
}

// Drive function
template<class T, class K>
typename QuadTree<T,K>::point_id QuadTree<T,K>::search_point(value_t x, value_t y) {
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = x;
	p->y = y;

	point_id r = search_point(p,center_x,center_y,Lx,Ly);
	
	returnavail(p);
	return r;
}

template<class T, class K>
typename QuadTree<T,K>::point_id QuadTree<T,K>::search_point(QuadTreeNode *p, value_t x, value_t y, double lx, double ly) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	// If there is no node root
	if (root==nullptr) return point_id();
	
	// If root is leaf check if the position matches the point
	if (root->type == QuadTreeNode::BLACK) {
		if (fabs(root->x-p->x)<0.00001 && fabs(root->y-p->y)<0.00001)
			return root->info;
		else
			return point_id();
	} 

	// Search for a leaf
	QuadTreeNode *t = root;
	Quadrant q = compare(p,x,y);
	while (t->children[q] != nullptr && t->children[q]->type == QuadTreeNode::GREY) {
		t = t->children[q];
		x += XF[q]*lx;
		y += YF[q]*ly;
		lx /= 2.0;
		ly /= 2.0;
		q = compare(p,x,y);
	}

	if (t->children[q] == nullptr || fabs(t->children[q]->x-p->x)>=0.00001 || fabs(t->children[q]->y-p->y)>=0.00001) 
		return point_id();
	else 
		return t->children[q]->info;
}

// Drive function
template<class T, class K> template<class R>
void QuadTree<T,K>::search_region(value_t x, value_t y, value_t d_x, value_t d_y, R report) {

}

