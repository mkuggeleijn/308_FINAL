#pragma once

#include "vEvent.h"
#include "vArc.h"
#include "vEdge.h"
#include "vVertexPoint.h"
#include "cgra_math.hpp"


// ############################################################
// Borderline Parabola Arc Class 
// ############################################################

class ArcNode;
class vEvent;

class ArcNode {

private:
	vVertexPoint* site;
	ArcNode* left;
	ArcNode* right;
	ArcNode* parent;
	vEvent* event;
	vEdge* edge;


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
	vEdge* getEdge();

	ArcNode* getLeftChildLeaf(ArcNode*);
	ArcNode* getRightChildLeaf(ArcNode*);

	ArcNode* getLeftParent(ArcNode*);
	ArcNode* getRightParent(ArcNode*);

	ArcNode* getLeftLeaf(ArcNode*);
	ArcNode* getRightLeaf(ArcNode*);


	// Set
	void setSite(vVertexPoint*);
	void setLeft(ArcNode*);
	void setRight(ArcNode*);
	void setParent(ArcNode*);
	void setEvent(vEvent*);
	void setEdge(vEdge*);


};