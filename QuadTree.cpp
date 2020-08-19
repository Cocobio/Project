#include "QuadTree.hpp"

// Memory managment function for new nodes
// It can be modify to use a pool of nodes
template<class T, class K>
typename QuadTree<T,K>::QuadTreeNode* QuadTree<T,K>::new_node(typename QuadTreeNode::NodeType t) {
	QuadTreeNode* n = new QuadTreeNode;
	n->type = t;
	
	// Set all children to nullptr
	for(int i=0; i<4; i++)
		n->children[i] = nullptr;

	// count the new node
	node_count++;

	return n;
}

// Memory managment function
template<class T, class K>
void QuadTree<T,K>::returnavail(QuadTreeNode* p) {
	// Delete children in case there are any
	for (int i=0; i<4; i++)
		if (p->children[i] != nullptr) returnavail(p->children[i]);

	// Decrease the node counter and free de memory
	node_count--;
	free(p);
}

// Identify the quadrant
template<class T, class K>
typename QuadTree<T,K>::Quadrant QuadTree<T,K>::compare(QuadTreeNode *p, double x, double y) {
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

// Insert point drive. will return true if the point was inserted
template<class T, class K>
bool QuadTree<T,K>::insert(value_t x, value_t y, point_id p_id) {
	// Create a node for the point and sets its values
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = x;
	p->y = y;
	p->info = p_id;

	if (insert(p,center_x,center_y,Lx,Ly))
		return true;
	
	// If not inserted, free memory
	returnavail(p);
	return false;
}

// Insert point on tree
// returns whether the point was inserted or not
template<class T, class K>
bool QuadTree<T,K>::insert(QuadTreeNode *p, double x, double y, double lx, double ly) {
	// factors to move center of regions while the algorithm moves down the tree
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};
	
	// If there is no node root insert node as root
	if (root==nullptr) {
		root = p;
		_size++;
		return true;
	}
	
	// variables
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
	else if (t->children[q]->x == p->x && t->children[q]->y == p->y) return false;
	// else if (fabs(t->children[q]->x-p->x)<EPSILON && fabs(t->children[q]->y-p->y)<EPSILON) return false;
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

	_size++;
	return true;
}

// Drive functions
template<class T, class K>
bool QuadTree<T,K>::remove(value_t x, value_t y) {
	QuadTreeNode *p = new_node(QuadTreeNode::BLACK);
	p->x = x;
	p->y = y;

	bool removed = remove(p,center_x,center_y,Lx,Ly);
	returnavail(p);

	return removed;
}

// Delete node from tree
template<class T, class K>
bool QuadTree<T,K>::remove(QuadTreeNode *p, double x, double y, double lx, double ly) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	// if quadtree is empty
	if (root==nullptr) return false;

	// if quadtree size is 1
	else if (root->type == QuadTreeNode::BLACK) {
		if (fabs(root->x-p->x)<EPSILON && fabs(root->y-p->y)<EPSILON) {
			returnavail(root);
			root = nullptr;
			_size--;
			return true;
		}
		return false;
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


	if (t==nullptr || fabs(t->x-p->x)>=EPSILON || fabs(t->y-p->y)>=EPSILON)
		return false;

	// The point is contained and will be deleted
	_size--;

	returnavail(t);
	ft->children[q] = nullptr;
	unsigned s = 0;

	for (int i=0; i<4; i++) {
		if (ft->children[i]!=nullptr) {
			if (ft->children[i]->type == QuadTreeNode::GREY) return true;
			else s++;
		}
	}

	if (s>1) return true;

	t = f==nullptr ? root : f->children[qf];

	while (t!=nullptr && t->type == QuadTreeNode::GREY) {
		q = NW;
		for (int i = 0; t->children[i]==nullptr; i++)
			q = static_cast<Quadrant>(i+1);

		tmp = t->children[q];
		t->children[q] = nullptr;
		returnavail(t);
		t = tmp;

	}

	if (f==nullptr) root = t;
	else f->children[qf] = t;

	return true;
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
typename QuadTree<T,K>::point_id QuadTree<T,K>::search_point(QuadTreeNode *p, double x, double y, double lx, double ly) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	// If there is no node root
	if (root==nullptr) return point_id();
	
	// If root is leaf check if the position matches the point
	if (root->type == QuadTreeNode::BLACK) {
		if (fabs(root->x-p->x)<EPSILON && fabs(root->y-p->y)<EPSILON)
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

	if (t->children[q] == nullptr || fabs(t->children[q]->x-p->x)>=EPSILON || fabs(t->children[q]->y-p->y)>=EPSILON) 
		return point_id();
	else 
		return t->children[q]->info;
}

// Drive function
template<class T, class K> template<class R>
void QuadTree<T,K>::search_region(value_t x, value_t y, value_t d_x, value_t d_y, R report) {
	// If there are no points on the tree, just return
	if (root==nullptr) return;

	// recursively search on the tree
	search_region(root,center_x,center_y,Lx,Ly,x,y,d_x,d_y,report);
}

// 
template<class T, class K> template<class R>
void QuadTree<T,K>::search_region(QuadTreeNode *r, double x, double y, double lx, double ly, value_t r_x, value_t r_y, value_t lr_x, value_t lr_y, R report) {
	static float XF[] = {-0.25,0.25,-0.25,0.25};
	static float YF[] = {0.25,0.25,-0.25,-0.25};

	if (r->type==QuadTreeNode::BLACK) {
		if (fabs(r->x-r_x)*2<=lr_x && fabs(r->y-r_y)*2<=lr_y)
			report(r->info);
		return;
	}

	// else GREY
	for (int i=0; i<4; i++) {
		if (r->children[i]!=nullptr && 
			fabs(x+XF[i]*lx-r_x)*2<=lx+lr_x && 
			fabs(y+YF[i]*ly-r_y)*2<=ly+lr_y)
			search_region(r->children[i],x+XF[i]*lx,y+YF[i]*ly,lx/2.0,ly/2.0,r_x,r_y,lr_x,lr_y,report);
	}
}

template<class T, class K>
void QuadTree<T,K>::clear() { 
	returnavail(root);
	root = nullptr;
	_size = 0;
}

template<class T, class K>
size_t QuadTree<T,K>::size() {
	return _size;
}
