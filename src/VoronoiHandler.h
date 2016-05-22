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
#include "vArc.h"
#include "vEvent.h"

using namespace std;
using namespace cgra;


// ############################################################
// Voronoi Handler Class
// ############################################################

class VoronoiHandler {

private:

	int pointDensity;
	// vector<vec2> pointSet;
	vector<vVertexPoint*> polyCenters;
	vector<vVertexPoint*> polyCorners;

	vec2 generatePoint();

	priority_queue<vEvent,vector<vEvent>,CompareY> eventQueue;

	void AddParabolaNode(vVertexPoint*);
	

	
public:

	VoronoiHandler();
	~VoronoiHandler();
	void generateVPolys(vector<vVertexPoint>*);
	vector<vVertexPoint> generatePointSet(int);

};