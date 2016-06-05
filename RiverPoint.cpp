#include "RiverPoint.h"

RiverPoint::RiverPoint(vVertexPoint *location) {
	this->location = location;
}

RiverPoint::RiverPoint(vVertexPoint *location, float intensity) {
	this->location = location;
	this->intensity = intensity;
}

void RiverPoint::updateFlow(int flow) {
	water += flow;
	// recalculate width too
	if (!exit) {
		downstream->updateFlow(flow);
	}

}


bool RiverPoint::isSource() {
	return source;
}
bool RiverPoint::isExit() {
	return exit;
}
float RiverPoint::getWater() {
	return water;
}
float RiverPoint::getWidth() {
	return width;
}
vVertexPoint* RiverPoint::getLocation() {
	return location;
}
RiverPoint* RiverPoint::getDownstream() {
	return downstream;
}

void RiverPoint::setSource(bool source) {
	this->source = source;
}
void RiverPoint::setExit(bool exit) {
	this->exit = exit;
}
void RiverPoint::setWater(float) {
	this->water = water;
}
void RiverPoint::setWidth(float) {
	this->width = width;
}
void RiverPoint::setLocation(vVertexPoint *) {
	this->location = location;
}
void RiverPoint::setDownstream(RiverPoint *) {
	this->downstream = downstream;
}