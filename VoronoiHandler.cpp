#include "VoronoiHandler.h"

// ############################################################
// Voronoi Handler Class
// ############################################################

VoronoiHandler::VoronoiHandler() {

}

// Constructor
VoronoiHandler::VoronoiHandler(int density) {
	this->triVertices = generatePointSet(density);
	this->triangles = generateTriangles(triVertices);
	this->triEdges = findEdges(triangles);
	this->borderTris = findBorders(triangles);
	cout << "Generated " << triangles.size() << " triangles, with ";
	cout << triEdges.size() << " edges." << endl;

	//Some debug stuff under here
	/*
	for (vTriangle* t : triangles) {
		triangleCount++;
		edgeCount += t->getEdges().size();
		cornerCount += t->getCorners().size();
		cout << "Triangle " << t << " with " << t->getEdges().size() << " edges, and " << t->getCorners().size() << " corners."<<endl;
		for (vVertexPoint *c : t->getCorners()) {
			cout << "\tCoords: " << c->getCoords();
			cout << "\tScreenCoords: " << c->screenCoords << endl;
		}
	}
*/

	this->polyVertices = findCenters(triangles);
	this->polyCenters = findPolyCenters(triVertices);
	this->polygons = generateVPolys(polyCenters);
	this->polyEdges = findEdges(polygons);
	this->borderPolys = findBorders(polygons);
	cout << "Generated " << polygons.size() << " polygons, with ";
	cout << polyEdges.size() << " edges." << endl;

	//pinToEdges(polyVertices);

	for (vTriangle *p : polygons) {
		p->updateCenter();
	}

	// More Debug stuff
	/*
	for (vTriangle* t : polygons) {
		triangleCount++;
		edgeCount += t->getEdges().size();
		cornerCount += t->getCorners().size();
		cout << "Polygon " << t << " with " << t->getEdges().size() << " edges, and " << t->getCorners().size() << " corners." << endl;
		for (vVertexPoint *c : t->getCorners()) {
			cout << "\tCoords: " << c->getCoords();
			cout << "\tScreenCoords: " << c->screenCoords << endl;
		}
	}
	*/

	// adjust centers - not working
	//for (vTriangle *p : polygons) {
	//	p->updateCenter();
	//}

	//for (int x = 0; x < relaxPasses; x++) {
	//	this->polyVertices = relaxTriangles(this->polyVertices, this->polygons);
	//}

	//removeBorderPolys(borderTris);
}

// Destructor
VoronoiHandler::~VoronoiHandler() {
	for (vTriangle* t : triangles) {
		delete t;
	}

	for (vEdge *e : triEdges) {
		delete e;
	}
	for (vVertexPoint *v : triVertices) {
		delete v;
	}

	for (vTriangle* t : polygons) {
		delete t;
	}

	for (vEdge *e : polyEdges) {
		delete e;
	}

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

vector<vVertexPoint*>  VoronoiHandler::findPolyCenters(vector<vVertexPoint*> triVerts) {
	vector<vVertexPoint*> centers;
	for (vVertexPoint *v : triVerts) {
		if (!v->isBorder())
			centers.push_back(v);
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
	return this->polyEdges;
}

// Generate (density) random points in a normal distribution
vector<vVertexPoint*> VoronoiHandler::generatePointSet(int density) {
	float max = 1.0;
	float min = 0.0;
	// vector<vec2> pointSet = makeCornerPoints(max, min);
	vector<vec2> pointSet;
	vector<vVertexPoint*> vertexSet;
	
	// pointSet.clear();

	std::random_device rd1;
	std::random_device rd2;
	std::mt19937 gen1(rd1());
	std::mt19937 gen2(rd2());
	std::uniform_real_distribution<> dis(min,max);
	// std::uniform_real_distribution<> disy(0, 1);

	for (int p = 0; p < density; p++) {
		
		float x = dis(gen1);
		float y = dis(gen2);
		//cout << "x = " << x << " \ty = " << y << endl;

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

// Make a set of points that defines four triangles centered around each corner of a square (0.0->0.1)
vector<vec2> VoronoiHandler::makeCornerPoints(float max, float min) {
	vector<vec2> pointSet;

	// First make corner points
	float cornerSize = 0.01;

	vec2 t0v1(min + cornerSize, min + cornerSize);
	vec2 t0v0(min - cornerSize, min + cornerSize);
	vec2 t0v2(min, min - cornerSize);

	vec2 t1v1(max + cornerSize, min + cornerSize);
	vec2 t1v0(max - cornerSize, min + cornerSize);
	vec2 t1v2(max, min - cornerSize);

	vec2 t2v1(min + cornerSize, max - cornerSize);
	vec2 t2v0(min - cornerSize, max - cornerSize);
	vec2 t2v2(min, max + cornerSize);

	vec2 t3v1(max + cornerSize, max - cornerSize);
	vec2 t3v0(max - cornerSize, max - cornerSize);
	vec2 t3v2(max, max + cornerSize);

	pointSet.push_back(t0v0);
	pointSet.push_back(t0v1);
	pointSet.push_back(t0v2);

	pointSet.push_back(t1v0);
	pointSet.push_back(t1v1);
	pointSet.push_back(t1v2);

	pointSet.push_back(t2v0);
	pointSet.push_back(t2v1);
	pointSet.push_back(t2v2);

	pointSet.push_back(t3v0);
	pointSet.push_back(t3v1);
	pointSet.push_back(t3v2);

	return pointSet;
}

// Generate a triangle mesh from a set of points
vector<vTriangle*> VoronoiHandler::generateTriangles(vector<vVertexPoint*> triCenters) {
	list<vTriangle*> newTriangles;
	list <vTriangle*> badTriangles;
	vector<vTriangle*> deleteTriangles;
	vector<vTriangle*> finalTriangles;

	vector<vEdge*> badEdges;
	vector<vVertexPoint*> badCorners;


	// add super-triangle (must be large enough to completely contain all the points in pointList)
	float min = -0.1;
	float max = 2.1;

	vVertexPoint *superv0 = new vVertexPoint(min, min);
	vVertexPoint *superv1 = new vVertexPoint(max, min);
	vVertexPoint *superv2 = new vVertexPoint(min, max);

	// cout << "Adding Super Triangle" << endl;
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
			//cout << "Adding New Triangle " << t << " with "<<t->getEdges().size()<<" edges."<< endl;
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

	//cout << "Total good triangles: " << finalTriangles.size() << endl;
	return finalTriangles;
}

// Check if a triangle shares a vertex with the super triangle (or any really)
bool VoronoiHandler::checkSuperTri(vTriangle *t, vTriangle *superTri) {
	//cout << "superTri corners: " << superTri->getCorners().size() << endl;
	for (vVertexPoint *v : t->getCorners()) {
		for (vVertexPoint *tv : superTri->getCorners()) {
			if (tv == v) {
				//cout << "Found triangle "<< t << "with vertex of super triangle" << endl;
				return true;
			}
		}
	}
	return false;
}

// Check if a point lies on the CircumCircle of a triangle
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

// Make a triangle mesh more regular
vector<vVertexPoint*> VoronoiHandler::relaxTriangles(vector<vVertexPoint*> points, vector<vTriangle*> polys) {
	float minV = 0.0;
	float maxV = 1.0;

	vec2 c0(minV, minV);
	vec2 c1(maxV, minV);
	vec2 c2(minV, maxV);
	vec2 c3(maxV, maxV);

	for (vVertexPoint *p : points) {
		// Only relax points that are not on the border. Might need special case for border points
		
		
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
			/*
			if (newX > maxV) newX = maxV;
			if (newX < minV) newX = minV;
			if (newY > maxV) newY = maxV;
			if (newY < minV) newY = minV;
			*/
			if (!p->isBorder()) {
				p->setCoords(newX, newY);
			}
			else {
				vec2 pc = p->getCoords();
				if (!(pc == c0 || pc == c1 || pc == c2 || pc == c3)) {

					if (pc.x == minV || pc.x == maxV) {
						p->setCoords(pc.x, newY);
					}
					if (pc.y == minV || pc.y == maxV) {
						p->setCoords(newX, pc.y);
					}
				}
			}


	}

	// Update the poly centers now the vertices have moved
	for (vTriangle *t : polys) {
		t->updateCenter();
	}

	//cout << "Number of border vertices: " << borderCount << endl;
	return points;
}

// Generate Voronoi Polygons from a triangle mesh
vector<vTriangle*> VoronoiHandler::generateVPolys(vector<vVertexPoint*> polyCenters) {
	vector<vTriangle*> triangles;

	for (vVertexPoint *p : polyCenters) {
		//cout << "Building polygon from " << p->getEdges().size() << " edges..." << endl;
		vTriangle *vPoly = new vTriangle(p,p->getEdges());
		//cout << "Created new polygon with " << vPoly->getCorners().size() << " vertices, " << vPoly->getEdges().size() << " edges." << endl;
		triangles.push_back(vPoly);
		}	
	return triangles;
}

// Given a polygon mesh, move all border vertices to the edges (range 0-1)
vector<vVertexPoint*> VoronoiHandler::pinToEdges(vector<vVertexPoint*> points) {
	
	float minV = 0.0;
	float maxV = 1.0;

	vec2 c0(minV, minV);
	vec2 c1(maxV, minV);
	vec2 c2(minV, maxV);
	vec2 c3(maxV, maxV);

	// Move border points to edges
	for (vVertexPoint* p : points) {
		if (p->isBorder()) {
			vec2 pc = p->getCoords();
			// If not one of the corner points
			if (!(pc == c0 || pc == c1 || pc == c2 || pc == c3)) {
				float px = p->getCoords().x;
				float py = p->getCoords().y;

				float farX = abs(maxV - px);
				float farY = abs(maxV - py);

				float dx = min(farX, px);
				float dy = min(farY, py);

				if (dx < dy) {
					if (farX < px) pc.x += dx;
					else pc.x -= dx;
				}
				else {
					if (farY < py) pc.y += dy;
					else pc.y -= dy;
				}
				p->setCoords(pc);
			}

		}

	}

	return points;
}

// Sample intensity of heightmap and add data to polygon graph
void VoronoiHandler::sampleImage(int imageSize, Image *heightMap) {
	for (vVertexPoint * p : triVertices) {
		int x = p->getCoords().x * (imageSize - 1);
		int y = p->getCoords().y * (imageSize - 1);

		p->screenCoords = vec2(x, y);
		p->setZValue(heightMap->getIntensity(x, y));
	}

	for (vVertexPoint * p : polyVertices) {
		int x = p->getCoords().x * (imageSize - 1);
		int y = p->getCoords().y * (imageSize - 1);
		p->screenCoords = vec2(x, y);
		p->setZValue(heightMap->getIntensity(x, y));
		cout << "PolyVertex at " << p->screenCoords << ", Intensity " << p->getZValue() << endl;
	}

	for (vVertexPoint * p : polyCenters) {
		int x = p->getCoords().x * (imageSize - 1);
		int y = p->getCoords().y * (imageSize - 1);
		p->screenCoords = vec2(x, y);
		p->setZValue(heightMap->getIntensity(x, y));
	}
}

void VoronoiHandler::rebuildPolyEdges() {

	for (vEdge *e : polyEdges) {
		delete e;
	}

	for (vTriangle *p : polygons) {

	}

}

void VoronoiHandler::removeBorderPolys(vector<vTriangle*> polys){

	vector<vEdge*> badEdges;
	vector<vVertexPoint*> badCorners;

	// Delete all border polys, mark edges & vertices for check
	for (vTriangle *p : polys) {
		for (vEdge *e : p->getEdges()) {
			if (find(badEdges.begin(), badEdges.end(), e) == badEdges.end())
				badEdges.push_back(e);
		}

		for (vVertexPoint *c : p->getCorners()) {
			if (find(badCorners.begin(), badCorners.end(), c) == badCorners.end())
				badCorners.push_back(c);
		}
		delete p;
	}

	// Delete all edges that go to border points
	for (vEdge *e : badEdges) {
		if (e->v0->isBorder() || e->v1->isBorder()) delete e;
	}

	// Delete all corners that are border points
	for (vVertexPoint *p : badCorners) {
		if (p->isBorder()) delete p;
	}
}
