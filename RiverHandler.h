#pragma once
#include <vector>
#include <map>
#include <random>
#include <cmath>


#include "RiverPoint.h"
#include "vVertexPoint.h"
#include "VoronoiHandler.h"
#include "simple_image.hpp"
#include "CImg.h"
#include "splineHandler.hpp"
#include "geometry.hpp"
//#include "riverData.hpp"

using namespace std;
using namespace cgra;
using namespace cimg_library;


class RiverHandler {
private:
	int numberOfRivers = 5;
	int cutoffPercent = 5;
	float startWater = 0.01;
	int waterScalar = 1;
	int widthScalar = 10;
	int density = 400;
	//Image heightMap = Image("./work/res/textures/test_heightmap.png");
	Image *heightMap;
	CImg<unsigned char> drawheightMap = CImg<unsigned char>("./work/res/textures/simplebump.pgm");
	int imageSize = drawheightMap.width();
	//int imageSize = 1024;
	Geometry *meshDisplay = nullptr;

	VoronoiHandler *graph;
	splineHandler *splineMaker;
	vector<vVertexPoint*> riverPoints;
	vector<vVertexPoint*> riverSources;
	vector<vector<vVertexPoint*>> rivers;

	//bool sortByZ(vVertexPoint*, vVertexPoint*) const;
	
	vector<vVertexPoint*> findSourceCandidates(vector<vVertexPoint*>);
	vector<vVertexPoint*> makeRiverPath(vVertexPoint*);
	vector<vector<vVertexPoint*>> makeRivers(int, vector<vVertexPoint*>);

	vVertexPoint* getNextRiverPoint(vVertexPoint*, vector<vVertexPoint*>*);
	vVertexPoint* dealWithLocalMin(vVertexPoint*, vector<vVertexPoint*>*);

	
	void drawEdges(vector<vEdge*>, CImg<unsigned char>*, const unsigned char[]);
	void drawPoints(vector<vVertexPoint*>, CImg<unsigned char>*, const unsigned char[], const unsigned char[],int);
	void drawRivers(vector<vector<vVertexPoint*>>, CImg<unsigned char>*, const unsigned char[], const unsigned char[], int);
	void drawRiverSplines(vector<vector<vVertexPoint*>>, CImg<unsigned char>*, const unsigned char[], const unsigned char[], int);
	
	
	void drawPolygons(vector<vTriangle*>, CImg<unsigned char>*, const unsigned char[], const unsigned char[], int);

	Geometry* makeGeo(vector<vTriangle*>);

public:
	RiverHandler();
	void drawRiversGL();
	void drawAll();
	Geometry* getGeo();
	Geometry* makeGeo();

	

	//vector <vector<river>> returnRiverData(vector<vector<vVertexPoint*>>);


};