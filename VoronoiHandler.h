#pragma once
#include <cmath>
#include <string>
#include <set>
#include <vector>
#include <random>
#include <queue>
#include <unordered_map>

#include "cgra_math.hpp"
#include "cgra_geometry.hpp"
#include "opengl.hpp"
#include "vVertexPoint.h"
#include "vEdge.h"
#include "vTriangle.h"



// #define EPSILON 1.0e-7

using namespace std;
using namespace cgra;

// ############################################################
// Voronoi Handler Class
// ############################################################

class VoronoiHandler {

private:

	int pointDensity;
	// vector<vec2> pointSet;
	vector<vVertexPoint*> triCenters;
	vector<vVertexPoint*> triCorners;
	vector<vEdge*> triEdges;
	list<vTriangle*> triangles;

	vector<vVertexPoint*> polyCenters;
	vector<vVertexPoint*> polyCorners;
	vector<vEdge*> polyEdges;


	vec2 generatePoint();
	static bool sortByX(const vec2, const vec2);
	bool circumCircle(vVertexPoint*, vTriangle*);
	bool checkSuperTri(vTriangle*, vTriangle*);


public:

	VoronoiHandler();
	~VoronoiHandler();
	vector<vTriangle*> generateTriangles(vector<vVertexPoint*>);
	vector<vVertexPoint*> relaxTriangles(vector<vVertexPoint*>);
	void generateVPolys(vector<vVertexPoint>*);
	vector<vVertexPoint*> generatePointSet(int);
	vector<vEdge*> getEdges();

};