#include "SimpleGrid.h"

SimpleGrid::SimpleGrid(int density) {
	pointGrid.clear();
	this->pointDensity = density;
	this->gridSize = 1.0 / (density-1);
	//generateGrid();
	makeGrid();

}

SimpleGrid::~SimpleGrid() {
	for (vector<vVertexPoint*>* row : pointGrid) {
		row->clear();
		}
}

vector<vector<vVertexPoint*>*>SimpleGrid::getPointGrid() {
	return this->pointGrid;
}

vector<vVertexPoint*>SimpleGrid::getPointCloud() {
	vector<vVertexPoint*> pointCloud;
	pointCloud.clear();

	for (vector<vVertexPoint*>* row : pointGrid) {
		for (int x = 0; x < row->size(); x++) {
			pointCloud.push_back(row->at(x));
		}
	}

	return pointCloud;
}

void SimpleGrid::makeGrid() {

	for (int y = 0; y < pointDensity; y++) {
		vector<vVertexPoint*> *row = new vector<vVertexPoint*>;
		row->clear();
		for (int x = 0; x < pointDensity; x++) {
			float px = x*gridSize;
			float py = y*gridSize;
			vVertexPoint *gridPoint = new vVertexPoint(px, py);
			if (x == 0 || y == 0 || x == gridSize - 1 || y == gridSize - 1) gridPoint->setExit(true);
			row->push_back(gridPoint);

			if (x > 0) {
				gridPoint->addEdgeNeighbour(row->at(x - 1));
				row->at(x - 1)->addEdgeNeighbour(gridPoint);
				if (y > 0) {
					gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x - 1));
					pointGrid.at(y - 1)->at(x - 1)->addEdgeNeighbour(gridPoint);
				}
			}
			if (y > 0) {
				gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x));
				pointGrid.at(y - 1)->at(x)->addEdgeNeighbour(gridPoint);
				if (x < gridSize - 1) {
					gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x + 1));
					pointGrid.at(y - 1)->at(x + 1)->addEdgeNeighbour(gridPoint);
				}
			}
			
		}
		pointGrid.push_back(row);
	}

}

void SimpleGrid::generateGrid() {

	for (int y = 0; y < pointDensity; y++) {
		vector<vVertexPoint*> *row = new vector<vVertexPoint*>;
		row->clear();
		for (int x = 0; x < pointDensity; x++) {
			// cout << "(" << x << "," << y << ")" << endl;
			float px = x*gridSize;
			float py = y*gridSize;
			vVertexPoint *gridPoint = new vVertexPoint(px, py);
			row->push_back(gridPoint);
			if (x > 0 && y > 0 && x!= pointDensity-1 && y != pointDensity-1) {
				gridPoint->addEdgeNeighbour(row->at(x - 1));
				gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x - 1));
				gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x));
				gridPoint->addEdgeNeighbour(pointGrid.at(y - 1)->at(x + 1));
			}
			
			if (y > 0 && x > 1 && y != pointDensity - 1) row->at(x - 1)->addEdgeNeighbour(gridPoint);

			if (y > 1) {
				if (x < pointDensity - 2) pointGrid.at(y - 1)->at(x + 1)->addEdgeNeighbour(gridPoint);
				if (x != pointDensity - 1 && x != 0) pointGrid.at(y - 1)->at(x)->addEdgeNeighbour(gridPoint);
				if (x > 1) pointGrid.at(y - 1)->at(x - 1)->addEdgeNeighbour(gridPoint);
			}
			
			// if (x > 1 && y > 1) pointGrid.at(y)->at(x - 1)->addEdgeNeighbour(gridPoint);

			
			
		}
		pointGrid.push_back(row);
	}

}