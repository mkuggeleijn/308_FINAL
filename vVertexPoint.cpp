#include "vVertexPoint.h"

// Constructors/Destructor

vVertexPoint::vVertexPoint() {
	coords = vec2(0, 0);
	zValue = 0.0f; 
	edges.clear();
	neighbours.clear();
}

vVertexPoint::vVertexPoint(vec2 coords) {
	this->coords = coords;
	zValue = 0.0f;
	edges.clear();
	neighbours.clear();
}

vVertexPoint::vVertexPoint(float x, float y) {
	this->coords = vec2(x, y);
	this->zValue = 0.0f;
	edges.clear();
	neighbours.clear();
}

vVertexPoint::vVertexPoint(vec2 coords, float zValue) {
	this->coords = coords;
	this->zValue = zValue;
	edges.clear();
	neighbours.clear();
}

vVertexPoint::vVertexPoint(float x, float y, float zValue) {
	this->coords = vec2(x, y);
	this->zValue = zValue;
	edges.clear();
	neighbours.clear();
}

vVertexPoint::~vVertexPoint() {}

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

vector<vVertexPoint*> vVertexPoint::getNeighbours() {
	return this->neighbours;
}
void vVertexPoint::addNeighbour(vVertexPoint* neighbour) {
	this->neighbours.push_back(neighbour);
}

void vVertexPoint::setExit(bool isExit) {
	this->isExit = isExit;
}

bool vVertexPoint::getExit() {
	return this->isExit;
}