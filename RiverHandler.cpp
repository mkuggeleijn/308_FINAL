#include "RiverHandler.h"

RiverHandler::RiverHandler(int numberOfRivers, int waterLevel, int widthFactor) {
	this->numberOfRivers = numberOfRivers;
	this->waterLevel = waterLevel;
	this->widthFactor = widthFactor;
	//riverSources = generateRivers(numberOfRivers);
}
/*
vector<RiverPoint*> RiverHandler::generateRivers(int numberOfRivers, vector<vVertexPoint*> sources) {

	map<int, vVertexPoint*> springs;
	springs.clear();

	int r = 0;
	while (r < numberOfRivers) {
		int springNumber = springs.size();
		int x = rand() % sources.size();
		springs.emplace(x, sources.at(x));
		if (springs.size() > springNumber) r++;
	}



}

RiverPoint* RiverHandler::findRiverPath(vVertexPoint *source, vector<vVertexPoint*> pointCloud) {

	RiverPoint *spring = new RiverPoint(source);
	spring->setWater(initalWater);

	float minZ = 0;
	for (vVertexPoint *p : source->getNeighbours()) {
		if (p->getZValue() < minZ) minZ = p->getZValue();
	}


}
*/