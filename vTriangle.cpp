#include "vTriangle.h"

vTriangle::vTriangle(vVertexPoint *center) {
	border = false;
	this->center = center;
	corners.clear();
	edges.clear();
	neighbours.clear();
}

vTriangle::~vTriangle() {
	// Remove self from all neighbours
	for (vTriangle* t : neighbours) {
		t->removeNeighbour(this);
	}
	// Remove self from all edges
	for (vEdge *e : edges) {
		e->removePoly(this);
	}
	// Remove self from all vertices
	for (vVertexPoint *c : corners) {
		c->removePoly(this);
	}
	// Remove center, it should have no edges at this stage ?????????
	delete center;
	
}


vTriangle::vTriangle(vVertexPoint *v1, vVertexPoint *v2, vVertexPoint *v3) {
	border = false;
	sortCounterClockwise(v1, v2, v3);

	v1->addPoly(this);
	v2->addPoly(this);
	v3->addPoly(this);

	neighbours.clear();
	edges.clear();

	vEdge *e0 = vEdge::checkDuplicate(v1, v2, this);
	vEdge *e1 = vEdge::checkDuplicate(v2, v3, this);
	vEdge *e2 = vEdge::checkDuplicate(v3, v1, this);

	edges.push_back(e0);
	edges.push_back(e1);
	edges.push_back(e2);

	float centerX = (v1->getCoords().x + v2->getCoords().x + v3->getCoords().x) / 3.0;
	float centerY = (v1->getCoords().y + v2->getCoords().y + v3->getCoords().y) / 3.0;

	this->center = new vVertexPoint(centerX, centerY);
	 
}


void vTriangle::sortCounterClockwise(vVertexPoint *v1, vVertexPoint *v2, vVertexPoint *v3) {
	vec2 v1v2 = v1->getCoords() - v2->getCoords();
	vec2 v2v3 = v2->getCoords() - v3->getCoords();
	float cross = (v1v2.x * v2v3.y) - (v1v2.y * v2v3.x);
	
	corners.clear();
	if (cross < 0.0) { // clockwise, need to be counter-clockwise
		corners.push_back(v3);
		corners.push_back(v2);
		corners.push_back(v1);
	}
	else {
		corners.push_back(v1);
		corners.push_back(v2);
		corners.push_back(v3);
	}

}

void vTriangle::removeNeighbour(vTriangle *t) {
	vector<vTriangle*>::iterator itr;
	itr = find(neighbours.begin(), neighbours.end(), t);
	if (itr != neighbours.end()) neighbours.erase(itr);
}

void vTriangle::updateBorder() {  
	// Triangles are on the border of the mesh if they have an edge with only one poly
	// Mark all vertices of these edges as border vertices
	for (vEdge *e : edges) {
		if (e->polys.size() < 2) {
			e->v0->setBorder(true);
			e->v1->setBorder(true);
		}
	}
}


vVertexPoint* vTriangle::getCenter() {
	return center;
}

vector<vVertexPoint*> vTriangle::getCorners() {
	return corners;
}
vector<vEdge*> vTriangle::getEdges() {
	return edges;
}
vector<vTriangle*> vTriangle::getNeighbours() {
	return neighbours;
}

void vTriangle::setCenter(vVertexPoint* center) {
	this->center = center;
}
void vTriangle::setCorners(vector<vVertexPoint*> corners) {
	this->corners = corners;
}
void vTriangle::setEdges(vector<vEdge*> edges) {
	this->edges = edges;
}
void vTriangle::setNeighbours(vector<vTriangle*> neighbours) {
	this->neighbours = neighbours;
}

void vTriangle::addCorner(vVertexPoint *corner) {
	corners.push_back(corner);
}
void vTriangle::addEdge(vEdge *edge) {
	edges.push_back(edge);
}
void vTriangle::addNeighbour(vTriangle *neighbour) {
	neighbours.push_back(neighbour);
}