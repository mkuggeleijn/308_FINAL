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
#include "vEdge.h"

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
	vector<vEdge*> polyEdges;

	ArcNode *treeRoot = nullptr;

	float sweepY; // Sweepline position

	vec2 generatePoint();

	set<vEvent*> deletedEvents;
	list<vVertexPoint*> newVPoints;
	priority_queue<vEvent*,vector<vEvent*>,CompareY> eventQueue;

	void addParabolaNode(vVertexPoint*);
	void deleteParabolaNode(vEvent*);

	ArcNode* getArcByX(float x);
	float getEdgeX(ArcNode*, float);
	float generateY(vVertexPoint*, float);
	void checkCircleEvent(ArcNode*);
	vVertexPoint* getEdgeIntersection(vEdge*, vEdge*);
	
public:

	VoronoiHandler();
	~VoronoiHandler();
	void generateVPolys(vector<vVertexPoint>*);
	vector<vVertexPoint> generatePointSet(int);
	vector<vEdge*> getEdges();

};