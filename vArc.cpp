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

vEdge* ArcNode::getEdge() {
	return edge;
}

void ArcNode::setSite(vVertexPoint *newSite) {
	site = newSite;
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

void ArcNode::setEdge(vEdge *newEdge) {
	edge = newEdge;
}


ArcNode* ArcNode::getLeftChildLeaf(ArcNode *node) {
	ArcNode *lchild = node->getLeft();
	while (!lchild->isLeaf) {
		lchild = lchild->getRight();
	}
	return lchild;
}

ArcNode* ArcNode::getRightChildLeaf(ArcNode *node) {
	ArcNode *rchild = node->getRight();
	while (!rchild->isLeaf) {
		rchild = rchild->getLeft();
	}
	return rchild;
}


ArcNode* ArcNode::getLeftParent(ArcNode *node) {
	ArcNode *nodeParent = node->getParent();
	ArcNode *lastNode = node;
	while (nodeParent->getLeft() == lastNode)
	{
		lastNode = nodeParent;
		nodeParent = nodeParent->getParent();
	}
	return nodeParent;
}

ArcNode* ArcNode::getRightParent(ArcNode *node) {
	ArcNode *nodeParent = node->getParent();
	ArcNode *lastNode = node;
	while (nodeParent->getRight() == lastNode)
	{
		lastNode = nodeParent;
		nodeParent = nodeParent->getParent();
	}
	return nodeParent;
}

ArcNode* ArcNode::getLeftLeaf(ArcNode *node) {
	return getLeftChildLeaf(getLeftParent(node));
}

ArcNode* ArcNode::getRightLeaf(ArcNode *node) {
	return getRightChildLeaf(getRightParent(node));
}

ArcNode::~ArcNode() {}