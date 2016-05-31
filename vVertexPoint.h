#pragma once

#include <vector>

#include "cgra_math.hpp"
#include "cgra_geometry.hpp"


using namespace std;
using namespace cgra;

// Class for vertex points

class vVertexPoint {
private:
	vec2 coords;
	vector<vVertexPoint*> edgeNeighbours;
	vector<vVertexPoint*> cellNeighbours;
	float zValue;
	bool isExit;

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
	vector<vVertexPoint*> getEdgeNeighbours();
	void addEdgeNeighbour(vVertexPoint*);
	vector<vVertexPoint*> getCellNeighbours();
	void addCellNeighbour(vVertexPoint*);
	void setExit(bool);
	bool getExit();
};