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
#include "simple_image.hpp"



// #define EPSILON 1.0e-7

using namespace std;
using namespace cgra;

// ############################################################
// Voronoi Handler Class
// ############################################################

class VoronoiHandler {

private:

	int density;
	int relaxPasses = 1;
	// vector<vec2> pointSet;

	// Triangle mesh vertices are polygon centers
	vector<vTriangle*> triangles;
	vector<vVertexPoint*> triVertices;
	vector<vEdge*> triEdges;
	vector<vTriangle*> borderTris;
	vector<vTriangle*> relaxedTris;

	// Polygon vertices are triangle centers
	vector<vTriangle*> polygons;
	vector<vVertexPoint*> polyVertices;
	vector<vEdge*> polyEdges;
	vector<vTriangle*> borderPolys;
	vector<vVertexPoint*> polyCenters;

	vec2 generatePoint();
	vector<vec2> makeCornerPoints(float, float);
	static bool sortByX(const vec2, const vec2);
	bool circumCircle(vVertexPoint*, vTriangle*);
	bool checkSuperTri(vTriangle*, vTriangle*);

	vector<vTriangle*> findBorders(vector<vTriangle*>);
	vector<vVertexPoint*> findCenters(vector<vTriangle*>);
	vector<vEdge*> findEdges(vector<vTriangle*>);
	vector<vVertexPoint*> findPolyCenters(vector<vVertexPoint*>);
	
	void removeBorderPolys(vector<vTriangle*>);
	vector<vVertexPoint*> pinToEdges(vector<vVertexPoint*>);

	void rebuildPolyEdges();


public:

	VoronoiHandler();
	VoronoiHandler(int);
	~VoronoiHandler();
	vector<vTriangle*> generateTriangles(vector<vVertexPoint*>);
	vector<vVertexPoint*> relaxTriangles(vector<vVertexPoint*>, vector<vTriangle*>);
	vector<vTriangle*> generateVPolys(vector<vVertexPoint*>);
	vector<vVertexPoint*> generatePointSet(int);

	vector<vTriangle*> getTriangles();
	vector<vVertexPoint*> getTriVertices();
	vector<vEdge*> getTriEdges();

	vector<vTriangle*> getPolygons();
	vector<vVertexPoint*> getPolyVertices();
	vector<vEdge*> getPolyEdges();

	void sampleImage(int,Image);

	void changeDensity(int);
	
	// void newMesh();

};