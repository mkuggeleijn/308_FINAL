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

	//// pointSet.clear();
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_real_distribution<> dis(0, 1);
	//float x = dis(gen);
	//float y = dis(gen);
	//vec2 point(x, y);

	return vec2::random(0.1, 0.4);;
}

bool VoronoiHandler::sortByX(const vec2 p1, const vec2 p2) {
	return p1.x < p2.x;
}

template <typename T>
struct pointer_values_equal
{
	const T* to_find;

	bool operator()(const T* other) const
	{
		return *to_find == *other;
	}
};

// ############################################################
// Public
// ############################################################

/*
Generate a cloud of points between 0 and 1
*/

vector<vEdge*> VoronoiHandler::getEdges() {
	return polyEdges;
}

vector<vVertexPoint*> VoronoiHandler::generatePointSet(int density) {

	vector<vec2> pointSet;
	vector<vVertexPoint*> vertexSet;
	
	pointSet.clear();

	std::random_device rd1;
	std::random_device rd2;
	std::mt19937 gen1(rd1());
	std::mt19937 gen2(rd2());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	// std::uniform_real_distribution<> disy(0, 1);

	for (int p = 0; p < density; p++) {
		
		float x = dis(gen1);
		float y = dis(gen2);
		cout << "x = " << x << " \ty = " << y << endl;

		// vVertexPoint point(x, y);
		vec2 vPoint(x, y);
		pointSet.push_back(vPoint);
	}

	sort(pointSet.begin(), pointSet.end(),sortByX); // ascending order by X

	for (vec2 p : pointSet) {
		vVertexPoint *v = new vVertexPoint(p.x, p.y);
		vertexSet.push_back(v);
	}
	return vertexSet; 

}

list<vTriangle*> VoronoiHandler::generateTriangles(vector<vVertexPoint*> triCenters) {
	list<vTriangle*> newTriangles;
	list <vTriangle*> badTriangles;
	list<vTriangle*> finalTriangles;

	// add super-triangle (must be large enough to completely contain all the points in pointList)
	float min = 0.0;
	float max = 2.0;

	vVertexPoint *superv0 = new vVertexPoint(min, min);
	vVertexPoint *superv1 = new vVertexPoint(max, min);
	vVertexPoint *superv2 = new vVertexPoint(min, max);

	cout << "Adding Super Triangle" << endl;
	vTriangle *superTri = new vTriangle(superv0, superv1, superv2);
	//cout << "SuperTri edge size: " << superTri->getEdges().size() << endl;

	newTriangles.push_back(superTri);
	
	// add the points one at a time to the triangulation
	for (vVertexPoint *v : triCenters) {
		badTriangles.clear();
		for (vTriangle *t : newTriangles) {
			if (circumCircle(v, t)) { //if point is inside circumcircle of triangle
				//cout << "point "<<v->getCoords()<< "inside triangle, adding to bad list" << endl;
				badTriangles.push_back(t);
			}
		}
		vector<vEdge*> holeEdges;

		// find the boundary of the polygonal hole
		for (vTriangle *t : badTriangles) {
			//cout << "edgesize: " << t->getEdges().size() << endl;
			for (vEdge *e : t->getEdges()) {
				// if edge is not shared by any other triangles in badTriangles
				bool shared = false;
				if (e->polys.size() > 0) {
					for (vTriangle *et : e->polys) {
						if (et != t && (find(badTriangles.begin(), badTriangles.end(), et) != badTriangles.end())) {
							//cout << "Found a shared edge in badTriangles" << endl;
							shared = true;
						}
					}
				}
				if (!shared) {
					//cout << "Adding edge to holeedge list" << endl;
					holeEdges.push_back(e);
				}
			}
		}

		// remove bad triangles from the data structure
		for (vTriangle *t : badTriangles) {
			//cout << "Removing bad triangle from list"<<endl;
			newTriangles.remove(t);
		}

		// re-triangulate the polygonal hole
		for (vEdge* e : holeEdges) { 
			vTriangle *t = new vTriangle(v, e->v0, e->v1); // form a triangle from edge to point
			//cout << "Adding New Triangle" << endl;
			newTriangles.push_back(t);
		}
	}
	
	// if triangle contains a vertex from original super-triangle 	remove triangle from list
	// Should be okay if make sure random points have some on border
	for (vTriangle *t : newTriangles) {
		if (!checkSuperTri(t, superTri)) finalTriangles.push_back(t);
	}

	cout << "Total triangles: " << finalTriangles.size() << endl;
	cout << "Bad triangles: " << badTriangles.size() << endl;
	return finalTriangles;
	// return badTriangles;
}

bool VoronoiHandler::checkSuperTri(vTriangle *t, vTriangle *superTri) {

	for (vVertexPoint *v : t->getCorners()) {
		for (vVertexPoint *tv : superTri->getCorners()) {
			if (tv == v) {
				cout << "Found triangle with vertex of super triangle" << endl;
				return true;
			}
		}
	}
	return false;
}

bool VoronoiHandler::circumCircle(vVertexPoint *point, vTriangle *triangle) {

	// First make a matrix from the triangle verticies
	// then calculate the determinate
	// if >0, return true;
	// else return false

	float px = point->getCoords().x;
	float py = point->getCoords().y;

	float pxsq = px*px;
	float pysq = py*py;

	float x0 = triangle->getCorners().at(0)->getCoords().x;
	float x1 = triangle->getCorners().at(1)->getCoords().x;
	float x2 = triangle->getCorners().at(2)->getCoords().x;

	float x0sq = x0*x0;
	float x1sq = x1*x1;
	float x2sq = x2*x2;

	float y0 = triangle->getCorners().at(0)->getCoords().y;
	float y1 = triangle->getCorners().at(1)->getCoords().y;
	float y2 = triangle->getCorners().at(2)->getCoords().y;

	float y0sq = y0*y0;
	float y1sq = y1*y1;
	float y2sq = y2*y2;

	float det = mat4::det3x3((x0 - px), (y0 - py), ((x0sq - pxsq) + (y0sq - pysq)),
		(x1 - px), (y1 - py), ((x1sq - pxsq) + (y1sq - pysq)),
		(x2 - px), (y2 - py), ((x2sq - pxsq) + (y2sq - pysq)));

	if (det > 0) return true; // >0 if points are sorted counterclockwise (which they should be, see triangle constructor)
	else return false;
}

void VoronoiHandler::generateVPolys(vector<vVertexPoint> *pointCloud) {
}

