#pragma once
#include <vector>
#include <map>

#include "RiverPoint.h"
#include "vVertexPoint.h"
#include "simple_image.hpp"

class RiverHandler {

private:
	int initalWater = 1;
	int numberOfRivers;
	int imageSize;
	int cutoff = 0.5;

	int waterLevel;
	int widthFactor;

	Image heightMap = nullptr;

	vector<RiverPoint*> candidates;
	vector<RiverPoint*> sources;
	//RiverPoint* findRiverPath(vVertexPoint*, vector<vVertexPoint*>);

public :
	RiverHandler(int, int, int);
	RiverHandler(Image, vector <vVertexPoint*>);
	//vector<RiverPoint*> generateRivers(int, vector<vVertexPoint*>);

};