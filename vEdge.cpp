#include "vEdge.h"


vEdge::vEdge(vVertexPoint *v0, vVertexPoint *v1, vTriangle *poly) {

	this->v0 = v0;
	this->v1 = v1;
	v0->addEdge(this);
	v1->addEdge(this);
	polys.clear();
	polys.push_back(poly);

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
