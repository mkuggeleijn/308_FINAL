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

using namespace std;
using namespace cgra;
using namespace cimg_library;


class RiverHandler {
private:
	int numberOfRivers = 1;
	int cutoffPercent = 15;
	int density = 100;
	//Image heightMap = Image("./work/res/textures/test_heightmap.png");
	Image heightMap = Image("./work/res/textures/simplebump.png");
	CImg<unsigned char> drawheightMap = CImg<unsigned char>("./work/res/textures/simplebump.pgm");
	int imageSize = drawheightMap.width();
	//int imageSize = 1024;

	VoronoiHandler *graph;
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
	void drawPolygons(vector<vTriangle*>, CImg<unsigned char>*, const unsigned char[], const unsigned char[], int);

public:
	RiverHandler();

	void drawAll();


};