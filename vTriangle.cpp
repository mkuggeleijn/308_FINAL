#include "vTriangle.h"

vTriangle::vTriangle(vVertexPoint *center) {
	this->center = center;
	corners.clear();
	edges.clear();
	neighbours.clear();
}

vTriangle::vTriangle(vVertexPoint *v1, vVertexPoint *v2, vVertexPoint *v3) {
	sortCounterClockwise(v1, v2, v3);
	neighbours.clear();
	edges.clear();
	vEdge *e0 = vEdge::checkDuplicate(v1, v2, this);
	vEdge *e1 = vEdge::checkDuplicate(v2, v3, this);
	vEdge *e2 = vEdge::checkDuplicate(v3, v1, this);

	edges.push_back(e0);
	edges.push_back(e1);
	edges.push_back(e2);
}

vTriangle::vTriangle(vVertexPoint *center, vVertexPoint *v1, vVertexPoint *v2, vVertexPoint *v3) {
	this->center = center;

	sortCounterClockwise(v1, v2, v3);

	edges.clear();
	vEdge *e0 = vEdge::checkDuplicate(v1, v2, this);
	vEdge *e1 = vEdge::checkDuplicate(v2, v3, this);
	vEdge *e2 = vEdge::checkDuplicate(v3, v1, this);

	edges.push_back(e0);
	edges.push_back(e1);
	edges.push_back(e2);

	neighbours.clear();

}

void vTriangle::sortCounterClockwise(vVertexPoint *v1, vVertexPoint *v2, vVertexPoint *v3) {
	vec2 v1v2 = v1->getCoords() - v2->getCoords();
	vec2 v2v3 = v2->getCoords() - v3->getCoords();
	float cross = (v1v2.x * v2v3.y) - (v1v2.y * v2v3.x);
	
	corners.clear();
	if (cross > 0.0) { // clockwise, need to be counter-clockwise
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