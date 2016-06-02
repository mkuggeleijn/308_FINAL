#pragma once
#include <vector>
#include <map>

#include "RiverPoint.h"
#include "vVertexPoint.h"

class RiverHandler {

private:
	int initalWater = 1;
	int numberOfRivers;
	int waterLevel;
	int widthFactor;

	vector<RiverPoint*> riverSources;
	//RiverPoint* findRiverPath(vVertexPoint*, vector<vVertexPoint*>);

public :
	RiverHandler(int, int, int);
	//vector<RiverPoint*> generateRivers(int, vector<vVertexPoint*>);

};