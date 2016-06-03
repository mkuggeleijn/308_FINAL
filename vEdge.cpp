#include "vEdge.h"


vEdge::vEdge(vVertexPoint *v0, vVertexPoint *v1, vTriangle *poly) {

	this->v0 = v0;
	this->v1 = v1;
	v0->addEdge(this);
	v1->addEdge(this);
	polys.clear();
	polys.push_back(poly);

}

vEdge::~vEdge() { // The only time we'd be deleting an edge is if it had no polygons
	for (vEdge *n : neighbours) {
		n->removeNeighbour(this);
	}
	v0->removeEdge(this);
	v1->removeEdge(this);
}


//	Check if edges are unique. If not, add poly to edge & return it

vEdge* vEdge::checkDuplicate(vVertexPoint *v0, vVertexPoint *v1, vTriangle *poly) {

	for (vEdge *e : v0->getEdges()) {
		if ((e->v0 == v0 && e->v1 == v1) || (e->v0 == v1 && e->v1 == v0)) {
			//cout << "Found duplicate edge" << endl;
			poly->addNeighbour(e->polys.front());
			e->polys.push_back(poly);
			return e;
		}
	}
	return new vEdge(v0, v1,poly);


}

// Remove a polygon from edge (ie; if it's been deleted)
void vEdge::removePoly(vTriangle* t){
	vector<vTriangle*>::iterator itr;
	itr = find(polys.begin(), polys.end(), t);
	if (itr != polys.end()) polys.erase(itr);
	if (polys.size() == 0) delete this; // Can't have an edge without an associated poly
}

void vEdge::removeNeighbour(vEdge *e) {
	vector<vEdge*>::iterator itr;
	itr = find(neighbours.begin(), neighbours.end(), e);
	if (itr != neighbours.end()) neighbours.erase(itr);
	}