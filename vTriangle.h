#pragma once

#ifndef VTRIANGLE_H
#define VTRIANGLE_H

#include <vector>


#include "vVertexPoint.h"
#include "vEdge.h"

using namespace std;

class vTriangle;
class vVertexPoint;
class vEdge;

class vTriangle{

private:
	vVertexPoint* center;
	vector<vVertexPoint*> corners;
	vector<vEdge*> edges;
	vector<vTriangle*> neighbours;
	void sortCounterClockwise(vVertexPoint*, vVertexPoint*, vVertexPoint*);

public:


	vTriangle(vVertexPoint*);
	vTriangle(vVertexPoint*, vVertexPoint*, vVertexPoint*);
	vTriangle(vVertexPoint*, vVertexPoint*, vVertexPoint*, vVertexPoint*);

	vVertexPoint* getCenter();
	vector<vVertexPoint*> getCorners();
	vector<vEdge*> getEdges();
	vector<vTriangle*> getNeighbours();

	void setCenter(vVertexPoint*);
	void setCorners(vector<vVertexPoint*>);
	void setEdges(vector<vEdge*>);
	void setNeighbours(vector<vTriangle*>);

	void addCorner(vVertexPoint*);
	void addEdge(vEdge*);
	void addNeighbour(vTriangle*);


};

#endif VTRIANGLE_H