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
	vTriangle *polyCenter;
	vector<vTriangle*> polys;
	vector<vEdge*> edges;
	vector<vVertexPoint*> neighbours;

	float zValue;
	float water;
	bool border;
	bool river;
	vVertexPoint* downstream;
	

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

	vector<vVertexPoint*> getNeighbours();
	void addNeighbour(vVertexPoint*);
	void removeNeighbour(vVertexPoint*);

	vector<vEdge*> getEdges();
	void addEdge(vEdge*);
	void removeEdge(vEdge*);

	vector<vTriangle*> getPolys();
	void addPoly(vTriangle*);
	void removePoly(vTriangle*);

	void setRiver(bool);
	bool isRiver();

	void setDownstream(vVertexPoint*);
	vVertexPoint* getDownstream();

	void setBorder(bool);
	bool isBorder();

	void setWater(float);
	float getWater();
	void updateFlow(float);

	void setPolyCenter(vTriangle*);
	void removePolyCenter(vTriangle*);
	vTriangle* getPolyCenter();

	vec2 screenCoords;
};

#endif VVERTEXPOINT_H