#include "vArc.h"

// ############################################################
// Borderline Parabola Arc Class 
// ############################################################

ArcNode::ArcNode() {
	site = 0;
	isLeaf = true;
	left = 0;
	right = 0;
	parent = 0;
	event = 0;
}

ArcNode::ArcNode(vVertexPoint *site) {
	this->site = site;
	isLeaf = true;
	ArcNode* left = 0;
	ArcNode* right = 0;
	ArcNode* parent = 0;
	vEvent* event = 0;
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
	newLeft->setParent(this);
}

void ArcNode::setRight(ArcNode *newRight) {
	right = newRight;
	newRight->setParent(this);
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
	while (lchild != 0 && !lchild->isLeaf) {
		lchild = lchild->getRight();
	}
	return lchild;
}

ArcNode* ArcNode::getRightChildLeaf(ArcNode *node) {
	ArcNode *rchild = node->getRight();
	while (rchild != 0 && !rchild->isLeaf) {
		rchild = rchild->getLeft();
	}
	return rchild;
}


ArcNode* ArcNode::getLeftParent(ArcNode *node) {
	ArcNode *nodeParent = node->getParent();
	cout << "node->getParent(): " << node->getParent() << endl;
	ArcNode *lastNode = node;
	if (nodeParent == 0) return nodeParent;

	while (nodeParent->getLeft() == lastNode)
	{
		lastNode = nodeParent;
		nodeParent = nodeParent->getParent();
		if (nodeParent == 0) break;
	}
	return nodeParent;
}

ArcNode* ArcNode::getRightParent(ArcNode *node) {  // This is fucking up?
	ArcNode *nodeParent = node->getParent();
	cout << "node->getParent(): " << node->getParent() << endl;
	ArcNode *lastNode = node;
	if (nodeParent == 0) return nodeParent;

	while (lastNode == nodeParent->getRight()) // this is failing, why? skipping ahead. 
	{
		lastNode = nodeParent;
		nodeParent = nodeParent->getParent();
		if (nodeParent == 0) break;
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