#pragma once

#ifndef VVERTEXPOINT_H
#define VVERTEXPOINT_H

#include <vector>

#include "cgra_math.hpp"
#include "cgra_geometry.hpp"
#include "vEdge.h"
#include "vTriangle.h"


using namespace std;
using namespace cgra;

// Class for vertex points
class vVertexPoint;
class vEdge;

class vVertexPoint {
private:
	vec2 coords;
	//vector<vVertexPoint*> neighbours;
	vector<vTriangle*> polys;
	vector<vEdge*> edges;
	float zValue;
	bool border;

public:
	// Constructors/Destructor
	vVertexPoint();
	vVertexPoint(vec2);
	vVertexPoint(float, float);
	vVertexPoint(vec2, float);
	vVertexPoint(float, float, float);
	~vVertexPoint();

	// Set/Get
	vec2 getCoords();
	void setCoords(vec2);
	void setCoords(float, float);
	float getZValue();
	void setZValue(float);
	vector<vEdge*> getEdges();
	void addEdge(vEdge*);
	void removeEdge(vEdge*);
	//vector<vVertexPoint*> getNeighbours();
	//void addNeighbour(vVertexPoint*);
	vector<vTriangle*> getPolys();
	void addPoly(vTriangle*);
	void removePoly(vTriangle*);
	void setBorder(bool);
	bool isBorder();
};

#endif VVERTEXPOINT_H