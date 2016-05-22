#pragma once

#include "vEvent.h"
#include "vArc.h"
#include "vVertexPoint.h"
#include "cgra_math.hpp"


// ############################################################
// Borderline Parabola Arc Class 
// ############################################################

class ArcNode {

private:
	vVertexPoint* site;
	ArcNode* left;
	ArcNode* right;
	ArcNode* parent;
	vEvent* event;


public:
	ArcNode();
	ArcNode(vVertexPoint*);
	~ArcNode();
	
	bool isLeaf;

	// Get
	vVertexPoint* getSite();
	ArcNode* getLeft();
	ArcNode* getRight();
	ArcNode* getParent();
	vEvent* getEvent();

	// Set
	void setLeft(ArcNode*);
	void setRight(ArcNode*);
	void setParent(ArcNode*);
	void setEvent(vEvent*);


};