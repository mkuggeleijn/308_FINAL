#include "vTriangle.h"

vTriangle::vTriangle(vVertexPoint *center) {
	border = false;
	this->center = center;
	corners.clear();
	edges.clear();
	neighbours.clear();
}

vTriangle::~vTriangle() {

	//std::cout << "deleting triangle " << this << std::endl;

	// Remove center, it should have no edges at this stage ?????????
	//delete center;

	// Remove self from all neighbours
	for (vTriangle* t : neighbours) {
		//std::cout << "removing neighbour from triangle " << t << std::endl;
		t->removeNeighbour(this);
	}
	// Remove self from all vertices
	for (vVertexPoint *c : corners) {
		c->removePoly(this);
	}

	// Remove self from all edges
	for (vEdge *e : edges) {
		e->removePoly(this);
	}

}

vTriangle::vTriangle(vVertexPoint *center, vector<vEdge*> edgelist) {
	//cout << "Processing " << edgelist.size() << " edges..."<<endl;
	border = false;
	this->center = center;
	for (vEdge *e : edgelist) {
		if (e->polys.size() != 2) {
			//cout << "ERROR: This edge has " << e->polys.size() << " poly neighbours!" << endl;
		} else {
			//cout << "Finding polycenters..." << endl;
			vVertexPoint *v0 = e->polys.at(0)->getCenter();
			vVertexPoint *v1 = e->polys.at(1)->getCenter();
			//cout << "v0: " << v0 << "\tv1: " << v1 << endl;
			vEdge *newEdge = vEdge::checkDuplicate(v0, v1, this);
			edges.push_back(newEdge);
			if (find(corners.begin(), corners.end(), v0) == corners.end()) {
				corners.push_back(v0);
				v0->addPoly(this);
			}
				
			if (find(corners.begin(), corners.end(), v1) == corners.end()) {
				corners.push_back(v1);
				v1->addPoly(this);
			}
		}
	}
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

	// updateCenter();
	
	//float centerX = (v1->getCoords().x + v2->getCoords().x + v3->getCoords().x) / 3.0;
	// float centerY = (v1->getCoords().y + v2->getCoords().y + v3->getCoords().y) / 3.0;
	
	
	float centerX = 0.0;
	float centerY = 0.0;

	for (vVertexPoint *p : corners) {
		centerX += p->getCoords().x;
		centerY += p->getCoords().y;
	}
	centerX = centerX / corners.size();
	centerY = centerY / corners.size();
	

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

void vTriangle::updateCenter() {
	// cout << "Corner size: " << corners.size() << endl;
	float centerX = 0.0;
	float centerY = 0.0;

	for (vVertexPoint *p : corners) {
		centerX += p->getCoords().x;
		centerY += p->getCoords().y;
	}
	centerX = centerX / corners.size();
	centerY = centerY / corners.size();

	this->center->setCoords(centerX, centerY);
}

void vTriangle::removeNeighbour(vTriangle *t) {
	vector<vTriangle*>::iterator itr;
	itr = find(neighbours.begin(), neighbours.end(), t);
	if (itr != neighbours.end()) neighbours.erase(itr);
}

void vTriangle::removeCorner(vVertexPoint *c) {
	vector<vVertexPoint*>::iterator itr;
	itr = find(corners.begin(), corners.end(), c);
	if (itr != corners.end()) corners.erase(itr);
	//delete this;
}

void vTriangle::removeEdge(vEdge *e) {
	vector<vEdge*>::iterator itr;
	itr = find(edges.begin(), edges.end(), e);
	if (itr != edges.end()) edges.erase(itr);
	//delete this;
}


bool vTriangle::updateBorder() {  
	// Triangles are on the border of the mesh if they have an edge with only one poly
	// Mark all vertices of these edges as border vertices
	for (vEdge *e : edges) {
		if (e->polys.size() < 2) {
			e->v0->setBorder(true);
			e->v1->setBorder(true);
			this->border = true;
		}
	}
	return this->border;
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