#pragma once

#include "vVertexPoint.h"
#include <vector>

using namespace std;

class SimpleGrid {

private:
	int pointDensity;
	float gridSize;
	vector<vector<vVertexPoint*>*> pointGrid;
	void generateGrid();
	void makeGrid();

public:
	SimpleGrid(int);
	~SimpleGrid();
	vector<vector<vVertexPoint*>*> getPointGrid();
	vector<vVertexPoint*> getPointCloud();

};