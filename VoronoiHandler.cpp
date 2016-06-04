#include "VoronoiHandler.h"

// ############################################################
// Voronoi Handler Class
// ############################################################

VoronoiHandler::VoronoiHandler() {

}

VoronoiHandler::VoronoiHandler(int density) {
	this->triVertices = generatePointSet(density);
	this->triangles = generateTriangles(triVertices);
	this->triEdges = findEdges(triangles);
	this->borderTris = findBorders(triangles);

	for (int x = 0; x < relaxPasses; x++) {
		this->triVertices = relaxTriangles(this->triVertices, this->triangles);
	}

	this->polyVertices = findCenters(triangles);
	this->polygons = generateVPolys(triVertices);
	this->polyEdges = findEdges(polygons);
	this->borderPolys = findBorders(polygons);
}

VoronoiHandler::~VoronoiHandler() {

}


// ############################################################
// Private
// ############################################################

vector<vTriangle*> VoronoiHandler::findBorders(vector<vTriangle*> triangles) {
	vector<vTriangle*> borderPolys;
	for (vTriangle *t : triangles) {
		if (t->updateBorder()) borderPolys.push_back(t);
	}
	return borderPolys;
}

vector<vVertexPoint*>  VoronoiHandler::findCenters(vector<vTriangle*> polys) {
	vector<vVertexPoint*> centers;
	for (vTriangle *p : polys) {
		centers.push_back(p->getCenter());
	}
	return centers;
}

vector<vEdge*> VoronoiHandler::findEdges(vector<vTriangle*> polys){
	vector<vEdge*> edges;
	for (vTriangle *p : polys) {
		for (vEdge *e : p->getEdges()) {
			if (find(edges.begin(), edges.end(), e) == edges.end())
				edges.push_back(e);
		}
	}
	return edges;
}

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

vector<vTriangle*> VoronoiHandler::getTriangles() {
	return this->triangles;
}
vector<vVertexPoint*> VoronoiHandler::getTriVertices(){
	return this->triVertices;
}
vector<vEdge*> VoronoiHandler::getTriEdges() {
	return this->triEdges;
}

vector<vTriangle*> VoronoiHandler::getPolygons() {
	return this->polygons;
}
vector<vVertexPoint*> VoronoiHandler::getPolyVertices(){
	return this->polyVertices;
}

vector<vEdge*> VoronoiHandler::getPolyEdges() {
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

vector<vTriangle*> VoronoiHandler::generateTriangles(vector<vVertexPoint*> triCenters) {
	list<vTriangle*> newTriangles;
	list <vTriangle*> badTriangles;
	vector<vTriangle*> deleteTriangles;
	vector<vTriangle*> finalTriangles;

	vector<vEdge*> badEdges;
	vector<vVertexPoint*> badCorners;

	// add super-triangle (must be large enough to completely contain all the points in pointList)
	float min = 0.0;
	float max = 2.0;

	vVertexPoint *superv0 = new vVertexPoint(min, min);
	vVertexPoint *superv1 = new vVertexPoint(max, min);
	vVertexPoint *superv2 = new vVertexPoint(min, max);

	cout << "Adding Super Triangle" << endl;
	vTriangle *superTri = new vTriangle(superv0, superv1, superv2);
	//cout << "SuperTri: " << superTri << endl;

	newTriangles.push_back(superTri);
	
	// add the points one at a time to the triangulation
	for (vVertexPoint *v : triCenters) {
		badTriangles.clear();

		//if point is inside circumcircle of triangle, add it to the bad list
		for (vTriangle *t : newTriangles) {
			if (circumCircle(v, t)) { 
				//cout << "Adding Triangle to bad list; " << t << endl;
				badTriangles.push_back(t);
			}
		}

		vector<vEdge*> holeEdges;

		// find the boundary of the polygonal hole
		for (vTriangle *t : badTriangles) {
			// if edge is not shared by any other triangles in badTriangles, it's part of the hole
			for (vEdge *e : t->getEdges()) {	
				bool shared = false;
				if (e->polys.size() > 0) {
					for (vTriangle *et : e->polys) {
						if (et != t && (find(badTriangles.begin(), badTriangles.end(), et) != badTriangles.end())) {
							shared = true;
						}
					}
				}
				if (!shared) {
					// Adding edge to holeedge list
					holeEdges.push_back(e);
				}
			}
		}

		// remove bad triangles from the data structure
		for (vTriangle *t : badTriangles) {
			//cout << "Removing bad triangle from list; " << t << endl;
			newTriangles.remove(t);
			deleteTriangles.push_back(t);
		}

		// re-triangulate the polygonal hole
		for (vEdge* e : holeEdges) { 
			vTriangle *t = new vTriangle(v, e->v0, e->v1); // form a triangle from edge to point
			//cout << "Adding New Triangle; " << t << endl;
			newTriangles.push_back(t);
		}

	}
	
	// if triangle contains a vertex from original super-triangle remove triangle from list
	for (vTriangle *t : newTriangles) {
		if (checkSuperTri(t, superTri)) deleteTriangles.push_back(t);
	}

	// Tidy up all triangles to be deleted.  
	// Need to have a list of all their edges & corners for proper disposal
	for (vTriangle *t : deleteTriangles) {
		//cout << "Removing bad triangle from list; " << t << endl;
		newTriangles.remove(t);

		for (vEdge *e : t->getEdges()) {
			if(find(badEdges.begin(), badEdges.end(), e) == badEdges.end())
				badEdges.push_back(e);
		}

		for (vVertexPoint *c : t->getCorners()) {
			if (find(badCorners.begin(), badCorners.end(), c) == badCorners.end())
				badCorners.push_back(c);
		}
		delete t;
	}
	
	// Tidy up all badEdges without associated polygons
	for (vEdge *e : badEdges) {
		if (e->polys.size() == 0)
			delete e;
	}

	// Tidy up all badCorners without associated polygons
	for (vVertexPoint *c : badCorners) {
		if (c->getPolys().size() == 0)
			delete c;
	}

	// Mark all border polygons and make final list
	for (vTriangle *t : newTriangles) {
		t->updateBorder();
		finalTriangles.push_back(t);
	}

	cout << "Total good triangles: " << finalTriangles.size() << endl;
	return finalTriangles;
}

bool VoronoiHandler::checkSuperTri(vTriangle *t, vTriangle *superTri) {
	cout << "superTri corners: " << superTri->getCorners().size() << endl;
	for (vVertexPoint *v : t->getCorners()) {
		for (vVertexPoint *tv : superTri->getCorners()) {
			if (tv == v) {
				cout << "Found triangle "<< t << "with vertex of super triangle" << endl;
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

vector<vVertexPoint*> VoronoiHandler::relaxTriangles(vector<vVertexPoint*> points, vector<vTriangle*> polys) {
	//int borderCount = 0;
	for (vVertexPoint *p : points) {
		// Only relax points that are not on the border. Might need special case for border points
		
		//if (!p->isBorder()) {
			// Get centroid
			float centerX = 0.0;
			float centerY = 0.0;
			int validCount = 0;
			for (vTriangle *t : p->getPolys()){
					centerX += t->getCenter()->getCoords().x;
					centerY += t->getCenter()->getCoords().y;
			}
			
			centerX = centerX / p->getPolys().size();
			centerY = centerY / p->getPolys().size();

			// move point to half way between current position and centroid;
			float newX = (centerX - p->getCoords().x) / 2;
			float newY = (centerY - p->getCoords().y) / 2;
			p->setCoords(newX, newY);

			
		//} else {
		//	borderCount++;
		//}
	}

	// Update the poly centers now the vertices have moved
	for (vTriangle *t : polys) {
		t->updateCenter();
	}

	//cout << "Number of border vertices: " << borderCount << endl;
	return points;
}


vector<vTriangle*> VoronoiHandler::generateVPolys(vector<vVertexPoint*> pointCloud) {

	vector<vTriangle*> triangles;

	for (vVertexPoint *p : pointCloud) {
		if (!p->isBorder()) {
			vTriangle *vPoly = new vTriangle(p);

			for (int x = 0; x < p->getPolys().size(); x++) {
				vTriangle *poly1 = p->getPolys().at(x);
				vPoly->addCorner(poly1->getCenter());

				vTriangle *poly2;
				if (x < p->getPolys().size() - 1) {
					poly2 = p->getPolys().at(x + 1);
					vPoly->addCorner(poly2->getCenter());
				}
				else {
					poly2 = p->getPolys().at(0);
				}

				vEdge *edge = vEdge::checkDuplicate(poly1->getCenter(), poly2->getCenter(), vPoly);
			}
			triangles.push_back(vPoly);
		}
	}
	return triangles;

}

