#include "splineHandler.hpp"

splineHandler::splineHandler()
{
	// clearControlPoints();

}


splineHandler::~splineHandler()
{
}

/*
std::vector<vec4> splineHandler::getSplinePoints() {
	return splinePoints;
}

vec4 splineHandler::getSplinePoint(int index) {
	return splinePoints.at(index);
}
*/

vec4 splineHandler::calculateVirtualPoint(vec4 p1, vec4 p2) {

	vec4 virtualPoint(p2.x+(p2.x - p1.x), p2.y+(p2.y - p1.y), p2.z+(p2.z - p1.z), 1);
	return virtualPoint;
}

vec4 splineHandler::calculateSplinePoint(std::vector<vec4> controls, float u) {

	// Qi(u) =  0.5.[u^3, u^2, u, 1].catmullRom.[pi-1, pi, pi+1, pi+2]
	// Q(u) = (x at u, y at u), where u is point on curve (eg u0 start, u1 end)
	// Assume that looking for segment between controls[1] and controls[2];

	// cout << "Calculating Spline point. u = " << u << endl;
	vec4 uu(u*u*u, u*u, u, 1);
	mat4 points(controls[0], controls[1], controls[2], controls[3]);
	vec4 qu;
	qu = 0.5f*uu*transpose(catmullRom)*transpose(points);
	return qu;

}

std::vector<vec4> splineHandler::calculateSplineSegment(std::vector<vec4> controls, std::vector<vec4> splinePoints, int position) {

	float iter = 1 / numberOfSamples;
	// std::vector<vec4> splinePoints;

	// float segmentLength = calculateChordSegment(chordIndex);  // the summed length of the vectors between four control points

	// float segmentStep = segmentLength / numberOfSamples;

	float u0 = 0.25;
	float u1 = 0.75;
	
	if (position == 0) {
		splinePoints.push_back(controls[1]);
	}

	for (float u = 0; u < 1; u+=iter) {
		vec4 qu = calculateSplinePoint(controls, u);
		// qu += controls[1];
		// cout << "SplinePoint: " << qu << endl;
		splinePoints.push_back(qu);
	}
	
	if (position == 2) {
		splinePoints.push_back(controls[2]);
	}
	return splinePoints;
}

std::vector<vec4> splineHandler::calculateSpline(std::vector<vec4> controlPoints) {
	// cout << "Calculating Spline... " << endl;
	// might need to alter this
	vector<vec4> splinePoints;

	int size = controlPoints.size();
	int position;

	for (int p = 0; p <= size - 4; p++) {
		std::vector<vec4> controls;
		controls.push_back(controlPoints[p]);
		controls.push_back(controlPoints[p+1]);
		controls.push_back(controlPoints[p+2]);
		controls.push_back(controlPoints[p+3]);

		if (p == 0) {
			position = 0;
		}
		else if (p == size - 4) {
			position = 2;
		}
		else {
			position = 1;
		}

		splinePoints = calculateSplineSegment(controls,splinePoints,position);

	}
	return splinePoints;
}



void splineHandler::addChordLength(vec4 p1, vec4 p2) {
	float chordLength = distance(p1, p2);
	chordLengths.push_back(chordLength);
}

void splineHandler::addChordSet(std::vector<vec4> controls) {
	for (int x = 0; x < controls.size() - 1; x++) {
		addChordLength(controls[x], controls[x+1]);
	}
}

float splineHandler::calculateChordSegment(int chordIndex) {
	float segmentLength = 0;
	// cout << "chordLengths.size(): " << chordLengths.size() << endl;
	for (int x = 0; x < 3; x++) {
		// cout << "index: " << chordIndex + x << endl;
		segmentLength += chordLengths[chordIndex + x];
	}			
	return segmentLength;
}

std::vector<vec4> splineHandler::clearTwoSegments(std::vector<vec4> splinePoints) {
	int lastIndex = numberOfSamples * 2;

	for (int x = 0; x < lastIndex+2; x++) {
		splinePoints.pop_back();
	}
	return splinePoints;
}




vector<vec4> splineHandler::makeRiverSpline(vector<vVertexPoint*> riverPoints) {
	// vector<vec2> riverSpline;

	// z-value will be used to interpolate water amount over spline.

	vector<vec4> controlPoints;

	float v0x = riverPoints.at(0)->getCoords().x - (riverPoints.at(1)->getCoords().x - riverPoints.at(0)->getCoords().x);
	float v0y = riverPoints.at(0)->getCoords().y - (riverPoints.at(1)->getCoords().y - riverPoints.at(0)->getCoords().y);
	vec4 v0(v0x, v0y, 0, 1);
	
	vec4 v1;
	if (riverPoints.back()->getDownstream() != nullptr) {
		vec2 coords = riverPoints.back()->getDownstream()->getCoords();
		v1 = vec4(coords.x, coords.y, riverPoints.back()->getWater(), 1);
	}
	else {
		float v1x = riverPoints.back()->getCoords().x + (riverPoints.back()->getCoords().x - riverPoints.at(riverPoints.size()-2)->getCoords().x);
		float v1y = riverPoints.at(0)->getCoords().y - (riverPoints.back()->getCoords().y - riverPoints.at(riverPoints.size() - 2)->getCoords().y);
		v1 = vec4(v1x, v1y, riverPoints.back()->getWater(), 1);
	}

	controlPoints.push_back(v0);

	for (vVertexPoint* p : riverPoints) {
		vec4 cp(p->getCoords().x, p->getCoords().y, p->getWater(), 1);
		controlPoints.push_back(cp);
	}

	controlPoints.push_back(v1);
	

	return calculateSpline(controlPoints);
}