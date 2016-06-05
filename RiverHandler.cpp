#include "RiverHandler.h"

RiverHandler::RiverHandler(int numberOfRivers, int waterLevel, int widthFactor) {
	this->numberOfRivers = numberOfRivers;
	this->waterLevel = waterLevel;
	this->widthFactor = widthFactor;
	//riverSources = generateRivers(numberOfRivers);
}

RiverHandler::RiverHandler(Image heightMap, vector<vVertexPoint*> pointCloud) {
	
	imageSize = 1024; // This needs to be changed

	this->heightMap = heightMap;


	vector<RiverPoint*> potentials;
	vector<RiverPoint*> candidates;

	for (vVertexPoint *p : pointCloud) {
		int screenX = p->getCoords().x * (imageSize - 1);
		int screenY = p->getCoords().y * (imageSize - 1);
		float intensity;

		if (screenX < 0 || screenX >= imageSize || screenY < 0 || screenY >= imageSize)
			intensity = 0.0f;
		else intensity = heightMap.getIntensity(screenX, screenY);

		RiverPoint *r = new RiverPoint(p, intensity);

		potentials.push_back(r);
		if (intensity > cutoff) candidates.push_back(r);
	}
	// cout << "Intensity: " << tex2.getIntensity(screenX, screenY) << endl;

	// Now sort list by intensity and add the cutoffPercent to the sources list.



}