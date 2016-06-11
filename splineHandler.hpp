#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>

#include "cgra_math.hpp"
#include "cgra_geometry.hpp"
#include "opengl.hpp"
#include "vVertexPoint.h"

using namespace std;
using namespace cgra;

class splineHandler
{
private:
	// std::vector<vec4> controlPoints;
	// std::vector<vec4> splinePoints;
	std::vector<float> chordLengths;
	//vec4 virtualStartPoint;
	//vec4 virtualEndPoint;
	vec4 defaultVirtualPoint = vec4(0.0, 0.0, 0.0, 1.0);
	int controlPointRadius = 5;
	int virtualPointRadius = 2;
	float numberOfSamples = 3;
	mat4 catmullRom = mat4(-1, 3, -3, 1,
		2, -5, 4, -1,
		-1, 0, 1, 0,
		0, 2, 0, 0);
	void addChordLength(vec4, vec4);
	void addChordSet(std::vector<vec4>);
	float calculateChordSegment(int);
	vec4 calculateSplinePoint(std::vector<vec4>, float);
	vec4 calculateVirtualPoint(vec4, vec4);
	std::vector<vec4> calculateSplineSegment(std::vector<vec4>, std::vector<vec4>,int);
	std::vector<vec4> calculateSpline(std::vector<vec4>);

public:
	splineHandler();
	~splineHandler();
	std::vector<vec4> clearTwoSegments(std::vector<vec4>);
	vector<vec4> makeRiverSpline(vector<vVertexPoint*>);
	vector<vVertexPoint*> makeRiverPointSpline(vector<vVertexPoint*>);
	int getSampleSize();
};

