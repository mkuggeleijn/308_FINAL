#include "vArc.h"

// ############################################################
// Borderline Parabola Arc Class 
// ############################################################

ArcNode::ArcNode() {
	site = nullptr;
	isLeaf = false;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	event = nullptr;
}

ArcNode::ArcNode(vVertexPoint *site) {
	this->site = site;
	isLeaf = true;
	ArcNode* left = nullptr;
	ArcNode* right = nullptr;
	ArcNode* parent = nullptr;
	vEvent* event = nullptr;
}

vVertexPoint* ArcNode::getSite() {
	return site;
}

ArcNode* ArcNode::getLeft() {
	return left;
}

ArcNode* ArcNode::getRight() {
	return right;
}

ArcNode* ArcNode::getParent() {
	return parent;
}

vEvent* ArcNode::getEvent() {
	return event;
}

void ArcNode::setLeft(ArcNode *newLeft) {
	left = newLeft;
}

void ArcNode::setRight(ArcNode *newRight) {
	right = newRight;
}

void ArcNode::setParent(ArcNode *newParent) {
	parent = newParent;
}

void ArcNode::setEvent(vEvent *newEvent) {
	event = newEvent;
}


ArcNode::~ArcNode() {}