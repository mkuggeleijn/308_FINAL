#include "RiverPoint.h"

RiverPoint::RiverPoint(vVertexPoint *location) {
	this->location = location;
	this->water = 1;
	this->riverExit = location->isBorder();
}

RiverPoint::RiverPoint(vVertexPoint *location, float intensity) {
	this->location = location;
	this->intensity = intensity;
	this->riverExit = location->isBorder();
}

void RiverPoint::updateFlow(int flow) {
	water += flow;
	// recalculate width too
	if (!riverExit) {
		downstream->updateFlow(flow);
	}

}


bool RiverPoint::isSource() {
	return this ->source;
}
bool RiverPoint::isExit() {
	return this -> riverExit;
}
float RiverPoint::getWater() {
	return this -> water;
}
float RiverPoint::getWidth() {
	return this -> width;
}
vVertexPoint* RiverPoint::getLocation() {
	return this->location;
}
RiverPoint* RiverPoint::getDownstream() {
	return this -> downstream;
}

void RiverPoint::setSource(bool source) {
	this->source = source;
}
void RiverPoint::setExit(bool exit) {
	this->riverExit = exit;
}
void RiverPoint::setWater(int water) {
	this->water = water;
}

void RiverPoint::addWater(int water) {
	this->water += water;
}

void RiverPoint::setWidth(float width) {
	this->width = width;
}
void RiverPoint::setLocation(vVertexPoint *location) {
	this->location = location;
}
void RiverPoint::setDownstream(RiverPoint *location) {
	this->downstream = downstream;
}