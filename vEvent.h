#pragma once

#include <iostream>

#include "vVertexPoint.h"
#include "vArc.h"

using namespace std;

class ArcNode;

// vEvent
class vEvent {
private:

	// float yValue;
	// Arc handler?

public:
	bool placeEvent;
	vVertexPoint* point;
	ArcNode* arc;

	vEvent(vVertexPoint*, bool);
	~vEvent();
	// bool operator()(vEvent&, vEvent&);
};


// Comparison Class

struct CompareY {
public:
	// Comparison Class
	bool CompareY::operator()(vEvent* const event1, vEvent* const event2) {
		return (event1->point->getCoords().y > event2->point->getCoords().y);  /// need to sort this out - const handling????
	}
};