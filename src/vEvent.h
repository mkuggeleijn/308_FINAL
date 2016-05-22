#pragma once

#include <iostream>

#include "vVertexPoint.h"
#include "vArc.h"

using namespace std;


// vEvent
class vEvent {
private:

	// float yValue;
	// Arc handler?

public:
	bool placeEvent;
	vVertexPoint* point;

	vEvent(vVertexPoint*, bool);
	~vEvent();
	// bool operator()(vEvent&, vEvent&);
};


// Comparison Class

struct CompareY {
public:
	// Comparison Class
	bool CompareY::operator()(const vEvent& event1, const vEvent& event2) {
		return (event1.point->getCoords().y > event2.point->getCoords().y);  /// need to sort this out - const handling????
	}
};