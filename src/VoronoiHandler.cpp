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



void VoronoiHandler::AddParabolaNode(vVertexPoint *pPoint) {

	ArcNode par(pPoint);

	/*
	if (par has its circle event, when it is removed form the beachline)
	remove this event form the queue
	priorityQueue.pop(??)
	new arcs a, b, c;
	b.site = u;
	a.site = c.site = par.site; // site of arc is a focus of arc
	xl, xr  = left and right edges, which comes from point on par under u
	xl is a normal to  (a.site, b.site);
	xr is a normal to (b.site, c.site);
	replace par by the sequence a, xl, b, xr, c
	CheckCircleEvent(a);
	CheckCircleEvent(c);
	*/
}

void VoronoiHandler::generateVPolys(vector<vVertexPoint>* pointCloud) {

	// Populate Event Queue
	for (vVertexPoint &p : *pointCloud) {
		vEvent event(&p, true);
		eventQueue.push(event);
	}
	while (!eventQueue.empty()) {
		vEvent e = eventQueue.top(); // now this is a problem
		if (e.placeEvent) {
			// AddParabola( e.point );
		}
		else {
			// RemoveParabola(e.point);
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

