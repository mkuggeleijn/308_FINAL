#include "vVertexPoint.h"

// Constructors/Destructor

vVertexPoint::vVertexPoint() {
	coords = vec2(0, 0);
	zValue = 0.0f; 
	edges.clear();
	//neighbours.clear();
	border = false;
}

vVertexPoint::vVertexPoint(vec2 coords) {
	this->coords = coords;
	zValue = 0.0f;
	edges.clear();
	//neighbours.clear();
	border = false;
}

vVertexPoint::vVertexPoint(float x, float y) {
	this->coords = vec2(x, y);
	this->zValue = 0.0f;
	edges.clear();
	//neighbours.clear();
	border = false;
}

vVertexPoint::vVertexPoint(vec2 coords, float zValue) {
	this->coords = coords;
	this->zValue = zValue;
	edges.clear();
	//neighbours.clear();
	border = false;
}

vVertexPoint::vVertexPoint(float x, float y, float zValue) {
	this->coords = vec2(x, y);
	this->zValue = zValue;
	edges.clear();
	//neighbours.clear();
	border = false;
}

vVertexPoint::~vVertexPoint() {

	for (vTriangle *p : polys) {
		p->removeCorner(this);
	}

	for (vEdge *e : edges) {
		e->removeVertex(this);
	}

} 



void vVertexPoint::removeEdge(vEdge* e) {
	vector<vEdge*>::iterator itr;
	itr = find(edges.begin(), edges.end(), e);
	if (itr != edges.end()) edges.erase(itr);
	// if (edges.size() == 0) delete this; // a vertex can be created without an edge, but can't be left without one.
}

// Set/Get
float vVertexPoint::getZValue() {
	return this->zValue;
}

void vVertexPoint::setZValue(float zValue) {
	this->zValue = zValue;
}

vec2 vVertexPoint::getCoords() {
	//cout << "Coords: " << coords << endl;
	return this->coords;
}
void vVertexPoint::setCoords(vec2 coords) {
	this->coords = coords;

}
void vVertexPoint::setCoords(float x, float y) {
	this->coords = vec2(x, y);
}

vector<vEdge*> vVertexPoint::getEdges() {
	return this->edges;
}
void vVertexPoint::addEdge(vEdge *edge) {
	this->edges.push_back(edge);
}

vector<vTriangle*> vVertexPoint::getPolys() {
	return this->polys;
}


void vVertexPoint::addPoly(vTriangle* poly) {
	this->polys.push_back(poly);
}

void vVertexPoint::removePoly(vTriangle* poly) {
	vector<vTriangle*>::iterator itr;
	itr = find(polys.begin(), polys.end(), poly);
	if (itr != polys.end()) polys.erase(itr);
	// if (polys.size() == 0) delete this;
}

void vVertexPoint::setBorder(bool border) {
	this->border = border;
}

bool vVertexPoint::isBorder() {
	return this->border;
}