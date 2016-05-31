#include "vVertexPoint.h"

// Constructors/Destructor

vVertexPoint::vVertexPoint() {
	this->coords = vec2(0, 0);
	this->zValue = 0.0f;
	this->edgeNeighbours.clear();
	this->cellNeighbours.clear();
}

vVertexPoint::vVertexPoint(vec2 coords) {
	this->coords = coords;
	this->zValue = 0.0f;
	this->edgeNeighbours.clear();
	this->cellNeighbours.clear();
}

vVertexPoint::vVertexPoint(float x, float y) {
	this->coords = vec2(x, y);
	this->zValue = 0.0f;
	this->edgeNeighbours.clear();
	this->cellNeighbours.clear();

}

vVertexPoint::vVertexPoint(vec2 coords, float zValue) {
	this->coords = coords;
	this->zValue = zValue;
	this->edgeNeighbours.clear();
	this->cellNeighbours.clear();
}

vVertexPoint::vVertexPoint(float x, float y, float zValue) {
	this->coords = vec2(x, y);
	this->zValue = zValue;
	this->edgeNeighbours.clear();
	this->cellNeighbours.clear();
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

vector<vVertexPoint*> vVertexPoint::getEdgeNeighbours() {
	return this->edgeNeighbours;
}
void vVertexPoint::addEdgeNeighbour(vVertexPoint* neighbour) {
	this->edgeNeighbours.push_back(neighbour);
}

vector<vVertexPoint*> vVertexPoint::getCellNeighbours() {
	return this->cellNeighbours;
}
void vVertexPoint::addCellNeighbour(vVertexPoint* neighbour) {
	this->cellNeighbours.push_back(neighbour);
}

void vVertexPoint::setExit(bool isExit) {
	this->isExit = isExit;
}

bool vVertexPoint::getExit() {
	return this->isExit;
}