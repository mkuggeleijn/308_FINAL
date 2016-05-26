#include "VoronoiHandler.h"

// ############################################################
// Voronoi Handler Class
// ############################################################

VoronoiHandler::VoronoiHandler() {

}

VoronoiHandler::~VoronoiHandler() {

}


// ############################################################
// Private
// ############################################################

vec2 VoronoiHandler::generatePoint() {

	// pointSet.clear();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);
	float x = dis(gen);
	float y = dis(gen);
	vec2 point(x, y);
	return point;
}

ArcNode* VoronoiHandler::getArcByX(float x){
	ArcNode * arc = treeRoot;
	float currentX = 0.0;

	while (!arc->isLeaf) 
	{
		currentX = getEdgeX(arc, sweepY); 
		if (currentX > x) {
			arc = arc->getLeft();
		}
		else {
			arc = arc->getRight();
		}
	}
	return arc;
}

float VoronoiHandler::generateY(vVertexPoint *pPoint, float x){
	float pointX = pPoint->getCoords().x;
	float pointY = pPoint->getCoords().y;

	float dy = 2.0 * (pointY - sweepY);
	double a1 = 1.0 / dy;
	double b1 = -2.0 * pointX / dy;
	double c1 = sweepY + dy / 4.0 + pointX * pointX / dy;

	return(a1*x*x + b1*x + c1);
}

float VoronoiHandler::getEdgeX(ArcNode *arc, float y) {

	ArcNode *left = arc->getLeftChildLeaf(arc);
	ArcNode *right = arc->getRightChildLeaf(arc);

	vVertexPoint *leftSite = left->getSite();
	vVertexPoint *rightSite = right->getSite();

	float leftX = leftSite->getCoords().x;
	float leftY = leftSite->getCoords().y;
	float rightX = rightSite->getCoords().x;
	float rightY = rightSite->getCoords().y;

	float dp = 2.0 * (leftY - y);
	float a1 = 1.0 / dp;
	float b1 = -2.0 * leftX / dp;
	float c1 = y + dp / 4 + leftX * leftX / dp;

	dp = 2.0 * (rightY - y);
	float a2 = 1.0 / dp;
	float b2 = -2.0 * rightX / dp;
	float c2 = sweepY + dp / 4 + rightX * rightX / dp;

	float a = a1 - a2;
	float b = b1 - b2;
	float c = c1 - c2;

	float disc = b*b - 4 * a * c;
	float x1 = (-b + std::sqrt(disc)) / (2 * a);
	float x2 = (-b - std::sqrt(disc)) / (2 * a);

	float ry;
	if (leftY < rightY) ry = std::max(x1, x2);
	else ry = std::min(x1, x2);

	return ry;

}


void VoronoiHandler::checkCircleEvent(ArcNode *arc) {

	ArcNode *leftParent = arc->getLeftParent(arc);
	ArcNode *rightParent = arc->getRightParent(arc);
	ArcNode *leftChild = arc->getLeftChildLeaf(leftParent);
	ArcNode *rightChild = arc->getRightChildLeaf(rightParent);

	if (leftChild == nullptr || rightChild == nullptr || leftChild->getSite() == rightChild->getSite()) {
		return;
	}
	else {
		vVertexPoint *edgeIntersect = getEdgeIntersection(leftParent->getEdge(), rightParent->getEdge());
		if (edgeIntersect == nullptr) {
			return;
		}
		else {
			float dx = leftChild->getSite()->getCoords().x - edgeIntersect->getCoords().x;
			float dy = leftChild->getSite()->getCoords().y - edgeIntersect->getCoords().x;

			float d = sqrt((dx * dx) + (dy * dy));

			if (edgeIntersect->getCoords().y - d >= sweepY) {
				return;
			}
			else {
				vEvent *newEvent = new vEvent(new vVertexPoint(edgeIntersect->getCoords().x, edgeIntersect->getCoords().y - d), false);
				newVPoints.push_back(newEvent->point);
				arc->setEvent(newEvent);
				newEvent->arc = arc;
				eventQueue.push(newEvent);
			}

		}
	}
}

vVertexPoint* VoronoiHandler::getEdgeIntersection(vEdge *e1, vEdge *e2) {
	float x = (e2->g - e1->g) / (e1->f - e2->f);
	float y = e1->f * x + e1->g;

	if ((x - e1->v1->getCoords().x) / e1->direction->getCoords().x < 0) return nullptr;
	if ((y - e1->v1->getCoords().y) / e1->direction->getCoords().y < 0) return nullptr;

	if ((x - e2->v1->getCoords().x) / e2->direction->getCoords().x < 0) return nullptr;
	if ((y - e2->v1->getCoords().y) / e2->direction->getCoords().y < 0) return nullptr;

	vVertexPoint *newPoint = new vVertexPoint(x, y);
	newVPoints.push_back(newPoint);
	return newPoint;
}

void VoronoiHandler::addParabolaNode(vVertexPoint *pPoint) {

	if (treeRoot == nullptr) {
		treeRoot = new ArcNode;
		return;
	}
	else if (treeRoot->isLeaf && (treeRoot->getSite()->getCoords().y - pPoint->getCoords().y < 1)) {

			vVertexPoint *focalPoint = treeRoot->getSite();
			treeRoot->isLeaf = false;
			treeRoot->setLeft(new ArcNode(focalPoint));
			treeRoot->setRight(new ArcNode(pPoint));

			vVertexPoint *s = new vVertexPoint((pPoint->getCoords().x + focalPoint->getCoords().x) / 2, 1.0f);
			newVPoints.push_back(s);

			if (pPoint->getCoords().x > focalPoint->getCoords().x) {
				treeRoot->setEdge(new vEdge(s, focalPoint, pPoint));
			}
			else {
				treeRoot->setEdge(new vEdge(s, pPoint, focalPoint));
			}

			polyEdges.push_back(treeRoot->getEdge());
			return;
		} 
	else {
		ArcNode *parPoint = getArcByX(pPoint->getCoords().x);

		if (parPoint->getEvent() != nullptr)
		{
			deletedEvents.insert(parPoint->getEvent());
			parPoint->setEvent(nullptr);
		}

		vVertexPoint *startPoint = new vVertexPoint(pPoint->getCoords().x, generateY(parPoint->getSite(), pPoint->getCoords().x));
		newVPoints.push_back(startPoint);

		vEdge *leftEdge = new vEdge(startPoint, parPoint->getSite(), pPoint);
		vEdge *rightEdge = new vEdge(startPoint, pPoint, parPoint->getSite());

		leftEdge->neighbour = rightEdge;
		polyEdges.push_back(leftEdge);

		parPoint->setEdge(rightEdge);
		parPoint->isLeaf = false;

		ArcNode *newArc0 = new ArcNode(parPoint->getSite());
		ArcNode *newArc1 = new ArcNode(pPoint);
		ArcNode *newArc2 = new ArcNode(parPoint->getSite());

		parPoint->setRight(newArc2);
		parPoint->setLeft(new ArcNode());
		parPoint->getLeft()->setEdge(leftEdge);

		parPoint->getLeft()->setLeft(newArc0);
		parPoint->getLeft()->setRight(newArc1);

		checkCircleEvent(newArc0);
		checkCircleEvent(newArc2);
		}
	
}

void VoronoiHandler::deleteParabolaNode(vEvent *pEvent) {
	ArcNode *eventArc = pEvent->arc;
	ArcNode *leftParent = eventArc->getLeftParent(eventArc);
	ArcNode *rightParent = eventArc->getRightParent(eventArc);
	ArcNode *leftChild = eventArc->getLeftChildLeaf(leftParent);
	ArcNode *rightChild = eventArc->getRightChildLeaf(rightParent);

	if (leftChild->getEvent() != nullptr) { 
		deletedEvents.insert(leftChild->getEvent()); 
		leftChild->setEvent(nullptr); 
	}
	if (rightChild->getEvent() != nullptr) { 
		deletedEvents.insert(rightChild->getEvent());
		rightChild->setEvent(nullptr);
	}

	vVertexPoint *newPoint = new vVertexPoint(pEvent->point->getCoords().x, generateY(eventArc->getSite(), pEvent->point->getCoords().x));
	newVPoints.push_back(newPoint);
	leftParent->getEdge()->v2 = newPoint;
	rightParent->getEdge()->v2 = newPoint;

	ArcNode *higherArc;
	ArcNode *parentArc = eventArc;

	while (parentArc != treeRoot) { 
		parentArc = parentArc->getParent();
		if (parentArc == leftParent) {
			higherArc = leftParent;
		}
		if (parentArc == rightParent) {
			higherArc = rightParent;
		}
	}

	higherArc->setEdge(new vEdge(newPoint, leftChild->getSite(), rightChild->getSite()));
	polyEdges.push_back(higherArc->getEdge());

	ArcNode *grandParent = eventArc->getParent()->getParent();

	if (eventArc->getParent->getLeft() == eventArc)
	{
		if (grandParent->getLeft() == eventArc->getParent) {
			grandParent->setLeft(eventArc->getParent->getRight());
		}
		if (grandParent->getRight() == eventArc->getParent) {
			grandParent->setRight(eventArc->getParent->getRight());
		}
	}
	else
	{
		if (grandParent->getLeft() == eventArc->getParent) {
			grandParent->setLeft(eventArc->getParent->getLeft());
		}
		if (grandParent->getRight() == eventArc->getParent) {
			grandParent->setRight(eventArc->getParent->getLeft());
		}
	}

	delete eventArc->getParent();
	delete eventArc;

	checkCircleEvent(leftChild);
	checkCircleEvent(rightChild);

}

void VoronoiHandler::generateVPolys(vector<vVertexPoint> *pointCloud) {

	// Populate Event Queue
	for (int x = 0; x < pointCloud->size(); x++) {
		vVertexPoint *vpoint = &(pointCloud->at(x));
		vEvent event(vpoint, true);
		eventQueue.push(&event);
		cout << "Pushing event to queue" << endl;
	}
	
	while (!eventQueue.empty()) {
		vEvent *e = eventQueue.top(); // now this is a problem
		if (e->placeEvent) {
			addParabolaNode(e->point);
		}
		else {
			// deleteParabolaNode(e.point);
		}
	}
	
} 




// ############################################################
// Public
// ############################################################

/*
Generate a cloud of points between 0 and 1
*/

vector<vVertexPoint> VoronoiHandler::generatePointSet(int density) {

	vector<vVertexPoint> pointSet;
	
	pointSet.clear();

	std::random_device rd1;
	std::random_device rd2;
	std::mt19937 gen1(rd1());
	std::mt19937 gen2(rd2());
	std::uniform_real_distribution<> dis(0, 1);
	// std::uniform_real_distribution<> disy(0, 1);

	for (int p = 0; p < density; p++) {
		
		float x = dis(gen1);
		float y = dis(gen2);
		cout << "x = " << x << " \ty = " << y << endl;
		
		vVertexPoint point(x, y);
		pointSet.push_back(point);
		
	}
	
	return pointSet;

}

