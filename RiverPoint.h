#pragma once

#include <vector>

#include "vVertexPoint.h"

using namespace std;

class RiverPoint;

class RiverPoint {
private:
	bool source;
	bool exit;
	float water;
	float width;
	vVertexPoint *location;
	RiverPoint *downstream;

public:
	RiverPoint(vVertexPoint*);

	void updateFlow(int);

	bool isSource();
	bool isExit();
	float getWater();
	float getWidth();
	vVertexPoint* getLocation();
	RiverPoint* getDownstream();

	void setSource(bool);
	void setExit(bool);
	void setWater(float);
	void setWidth(float);
	void setLocation(vVertexPoint *);
	void setDownstream(RiverPoint *);



};