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
	int numberOfRivers = 5;
	int cutoffPercent = 15;
	int density = 100;
	Image heightMap = Image("./work/res/textures/test_heightmap.png");
	int imageSize = 1024;

	VoronoiHandler graph;
	vector<vVertexPoint*> riverPoints;
	vector<vVertexPoint*> riverSources;
	vector<vector<RiverPoint*>> rivers;

	vector<vVertexPoint*> sampleImage(Image);

	//bool sortByZ(vVertexPoint*, vVertexPoint*) const;
	
	vector<vVertexPoint*> findSourceCandidates(vector<vVertexPoint*>);
	vector<RiverPoint*> makeRiverPath(vVertexPoint*);
	vector<vector<RiverPoint*>> makeRivers(int, vector<vVertexPoint*>);

	RiverPoint* getNextRiverPoint(RiverPoint*);
	vVertexPoint* dealWithLocalMin(RiverPoint*);

	void drawEdges(vector<vEdge*>, CImg<unsigned char>*, const unsigned char[]);
	void drawPoints(vector<vVertexPoint*>, CImg<unsigned char>*, const unsigned char[],int);
	void drawRivers(vector<vector<RiverPoint*>>, CImg<unsigned char>*, const unsigned char[], int);


public:
	RiverHandler();

	void drawAll();


};