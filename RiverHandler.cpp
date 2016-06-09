#include "RiverHandler.h"


RiverHandler::RiverHandler() {
	heightMap = new Image("./work/res/textures/simplebump.png");

	this -> graph = new VoronoiHandler(density);
	this->splineMaker = new splineHandler();
	cout << "Generating geometry..." << endl;
	this->meshDisplay = makeGeo(graph->getTriangles());

	graph -> sampleImage(imageSize, heightMap);
	this -> riverSources = findSourceCandidates(graph->getPolyVertices());
	cout << "Found " << riverSources.size() << " river source candidates." << endl;
	cout << "Making " << numberOfRivers << " rivers..." << endl;
	this->rivers = makeRivers(numberOfRivers, riverSources);
	cout << "Found " << rivers.size() << " rivers." << endl;
	//Some debug stuff under here
	/*
	for (vTriangle* t : graph->getTriangles()) {
		cout << "Triangle " << t << " with " << t->getEdges().size() << " edges, and " << t->getCorners().size() << " corners." << endl;
		for (vVertexPoint *c : t->getCorners()) {
			cout << "\tCoords: " << c->getCoords();
			cout << "\tScreenCoords: " << c->screenCoords << endl;
		}
	}
	*/
	
}

struct sortByGreaterZ {
	bool operator()(vVertexPoint *p1, vVertexPoint *p2) const {
		return p1->getZValue() > p2->getZValue();
	}
};

struct sortByLesserZ {
	bool operator()(vVertexPoint *p1, vVertexPoint *p2) const {
		return p1->getZValue() < p2->getZValue();
	}
};


// Get certain amount of highest points
vector<vVertexPoint*>RiverHandler::findSourceCandidates(vector<vVertexPoint*> riverPoints) {

	sort(riverPoints.begin(), riverPoints.end(), sortByGreaterZ());

	vector<vVertexPoint*>riverSources;

	int riverCutOff = riverPoints.size() * (cutoffPercent / 100.0);
	//cout << "riverCutOff: " << riverCutOff;
	if (riverCutOff > riverPoints.size()) riverCutOff = riverPoints.size();
	if (riverCutOff < 1) riverCutOff = 1;

	for (int x = 0; x < riverCutOff; x++) {
		riverSources.push_back(riverPoints.at(x));
	}

	
	return riverSources;
}

// Generate Rivers
vector<vector<vVertexPoint*>> RiverHandler::makeRivers(int numberOfRivers, vector<vVertexPoint*> riverSources) {

	vector<vector<vVertexPoint*>> rivers;

	if (riverSources.size() < numberOfRivers) numberOfRivers = riverSources.size();

	const int range = riverSources.size() - 1;
	std::random_device rd1;
	std::mt19937 gen1(rd1());
	std::uniform_real_distribution<> dis(0, range);

	for (int x = 0; x < numberOfRivers; x++) {
		int n = dis(gen1);
		// int n = rand() % (riverSources.size() - 1);
		vVertexPoint *source = riverSources.at(n);
		/*
		while (source->isRiver()) {
			cout << "In the river loop, n = " << n << endl;
			//n = dis(gen1);
			n = rand() % (riverSources.size() - 1);
			vVertexPoint *source = riverSources.at(n);
		}
		*/
		vector<vVertexPoint*> newRiver = makeRiverPath(source);
		rivers.push_back(newRiver);
	}
	cout << "Made " << rivers.size() << " rivers" << endl;
	return rivers;
}

// Generate a single river
vector<vVertexPoint*> RiverHandler::makeRiverPath(vVertexPoint* source) {

	source->setRiver(true);
	source->setWater(startWater);
	vector<vVertexPoint*> river;
	river.push_back(source);
	source->setDownstream(getNextRiverPoint(source, &river));
	return river;
}

// Find the next point of the river
vVertexPoint* RiverHandler::getNextRiverPoint(vVertexPoint *parent, vector<vVertexPoint*> *river) {

	//cout << "Node " << parent << ": Neighbours: " << parent->getNeighbours().size() << "\tEdges: " << parent->getEdges().size();
	//cout << "\tIntensity: " << parent->getZValue() << endl;

	vector<vVertexPoint*> neighbours = parent->getNeighbours();
	sort(neighbours.begin(), neighbours.end(), sortByLesserZ());
	
	vVertexPoint *next;
	int x = 0;
	while (x < neighbours.size()) {
		next = neighbours.at(x);
		if (find(river->begin(), river->end(), next) == river->end()) break;
		x++;
	}

	// If there are no valid candidates
	if (x == neighbours.size()) {
		cout << "No valid river points found" << endl;
		parent->setBorder(true);
		return 0;
	}

	// if (next->getZValue() > parent->getZValue()) local minimum, tricky

	next->setRiver(true);
	float water = distance(next->getCoords(), parent->getCoords())*waterScalar;
	next->updateFlow(water);
	river->push_back(next);

	//cout << "Riverpoint " << next << " at " << next->screenCoords << ": Intensity " << next->getZValue();
	//cout << ", isBorder " << next->isBorder() << endl;

	// If we've hit the mesh edge
	if (next->isBorder()) {
		cout << "Mesh edge reached" << endl;
		return next;
	}

	
	// If we're merging two rivers
	if (next->isRiver() && (find(river->begin(), river->end(), next) == river->end())) {
		cout << "Merging two rivers" << endl;
		return next;
	}

	next->setDownstream(getNextRiverPoint(next, river));
	return next;
}


// Deal with a local minimum zvalue.  Tricky.
//vVertexPoint* RiverHandler::dealWithLocalMin(vVertexPoint *parent) {

//}

void RiverHandler::drawAll() {
	//CImg stuff, needed for testing
	const unsigned char cRed[] = { 255,0,0 };
	const unsigned char cBlue[] = { 0,0,255 };
	const unsigned char cWhite[] = { 255,255,255 };
	const unsigned char cBlack[] = { 0,0,0 };
	const unsigned char cGrey[] = { 127,127,127 };
	const unsigned char cDarkGrey[] = { 64,64,64 };
	const unsigned char cYellow[] = { 255,255,0 };

	CImg<unsigned char>  pointDisplay(imageSize, imageSize, 1, 3, 0);
	// pointDisplay.assign("./work/res/textures/simplebump.pgm");
	int radius = 2;

	cout << "Found " << graph->getTriangles().size() << " triangles, with " << graph->getTriEdges().size() << " edges." << endl;
	//cout << "Found " << graph->getPolygons().size() << " polygons, with " << graph->getPolyEdges().size() << " edges." << endl;

	drawEdges(graph->getTriEdges(), &pointDisplay, cGrey);
	//drawEdges(graph->getPolyEdges(), &pointDisplay, cYellow);
	drawPoints(graph->getPolyVertices(), &pointDisplay, cRed ,cBlue, radius);
	//drawPoints(graph->getTriVertices(), &pointDisplay, cWhite, cYellow, radius);
	//drawPolygons(graph.getTriangles(), &pointDisplay, cGrey, cWhite, radius);
	// drawRivers(rivers, &pointDisplay, cWhite, cWhite,radius);
	drawRiverSplines(rivers, &pointDisplay, cWhite, cWhite, radius);

	CImgDisplay draw_disp(pointDisplay, "Raw Mesh");
	while (!draw_disp.is_closed()) {
		draw_disp.wait();
	}
}

void RiverHandler::drawEdges(vector<vEdge*> edges, CImg<unsigned char> *pointDisplay, const unsigned char color[]) {
	for (vEdge *e : edges) {
		//cout << e->v0->screenCoords << " -- " << e->v1->screenCoords << endl;

		int p0x = e->v0->screenCoords.x;
		int p0y = e->v0->screenCoords.y;
		int p1x = e->v1->screenCoords.x;
		int p1y = e->v1->screenCoords.y;

		int points[4] = { p0x, p0y, p1x, p1y };

		pointDisplay->draw_line(points[0], points[1], points[2], points[3], color);

	}
}
void RiverHandler::drawPoints(vector<vVertexPoint*> points, CImg<unsigned char> *pointDisplay, const unsigned char color1[], const unsigned char color2[],int radius) {
	for (vVertexPoint* p : points) {
		if (p->isBorder()) pointDisplay->draw_circle(p->screenCoords.x, p->screenCoords.y, radius, color2);
		else pointDisplay->draw_circle(p->screenCoords.x, p->screenCoords.y, radius, color1);
	}

}
void RiverHandler::drawRivers(vector<vector<vVertexPoint*>> riverSet, CImg<unsigned char> *pointDisplay, const unsigned char lineColor[], const unsigned char nodeColor[], int radius) {
	for (vector<vVertexPoint*> river : riverSet) {
		for (vVertexPoint* r : river) {
			//cout << "Riverpoint " << r << " at " << r->screenCoords << endl;
			int p0x = r->screenCoords.x;
			int p0y = r->screenCoords.y;
			pointDisplay->draw_circle(p0x, p0y, radius, nodeColor);
			if (!r->isBorder()) {
				int p1x = r->getDownstream()->screenCoords.x;
				int p1y = r->getDownstream()->screenCoords.y;
				int points[4] = { p0x, p0y, p1x, p1y };
				//cout << "Riverline: " << points << endl;
				pointDisplay->draw_line(points[0], points[1], points[2], points[3], lineColor);
			}
		}
	}

}


void RiverHandler::drawRiverSplines(vector<vector<vVertexPoint*>> riverSet, CImg<unsigned char> *pointDisplay, const unsigned char lineColor[], const unsigned char nodeColor[], int radius) {
	for (vector<vVertexPoint*> river : riverSet) {

		for (vVertexPoint* r : river) {
			//cout << "Riverpoint " << r << " at " << r->screenCoords << endl;
			int p0x = r->screenCoords.x;
			int p0y = r->screenCoords.y;
			pointDisplay->draw_circle(p0x, p0y, radius, nodeColor);
		}

		if (river.size() == 2) {
			vec2 p0 = river.at(0)->getCoords();
			vec2 p1 = river.at(1)->getCoords();
			pointDisplay->draw_line(p0.x, p0.y, p1.x, p1.y, lineColor);
		} else if (river.size() > 2) {
			vector<vec4> riverSpline = splineMaker->makeRiverSpline(river);
			for (int x = 1; x < riverSpline.size() - 3; x++) {
				vec4 p0 = riverSpline.at(x);
				vec4 p1 = riverSpline.at(x+1);

				int p0x = p0.x * (imageSize - 1);
				int p0y = p0.y * (imageSize - 1);
				int p1x = p1.x * (imageSize - 1);
				int p1y = p1.y * (imageSize - 1);

				pointDisplay->draw_line(p0x, p0y, p1x, p1y, lineColor);

			}
	
		}


	}

}

void RiverHandler::drawRiversGL(){

	for (vector<vVertexPoint*> river : rivers) {

		if (river.size() > 1) {
			vector<vec4> riverSpline = splineMaker->makeRiverSpline(river);
			for (int x = 1; x < riverSpline.size() - 3; x++) {
				vec4 p0 = riverSpline.at(x);
				vec4 p1 = riverSpline.at(x+1);
				float width = p0.z*widthScalar;

				glLineWidth(width);
				glBegin(GL_LINES);
					glVertex2f(p0.x, p0.y);
					glVertex2f(p1.x, p1.y);
				glEnd();


			}

		}


	}

}

void RiverHandler::drawPolygons(vector<vTriangle*> polys, CImg<unsigned char> *pointDisplay, const unsigned char lineColor[], const unsigned char nodeColor[], int radius) {
	//cout << "Drawing " << polys.size() << " polygons."<<endl;
	for (vTriangle *p : polys) {
		//cout << "Drawing " << p->getEdges().size() << " edges" << endl;
		for (vEdge *e : p->getEdges()) {
			//cout << e->v0->screenCoords << " -- " << e->v1->screenCoords << endl;

			int p0x = e->v0->screenCoords.x;
			int p0y = e->v0->screenCoords.y;
			int p1x = e->v1->screenCoords.x;
			int p1y = e->v1->screenCoords.y;

			int points[4] = { p0x, p0y, p1x, p1y };

			pointDisplay->draw_line(points[0], points[1], points[2], points[3], lineColor);

		}
	}
}

Geometry* RiverHandler::makeGeo(vector<vTriangle*> triangles) {

	vector<vector<vec3>> triData;
	/*
	for (vTriangle* t : triangles) {
		vector<vec3> data;

		// corners are sorted in anticlockwise, need clockwise for obj formatting
		vec2 p0 = t->getCorners().at(0)->getCoords();
		vec2 p1 = t->getCorners().at(1)->getCoords();
		vec2 p2 = t->getCorners().at(2)->getCoords();

		vec3 tp0(p0.x, p0.y, 0);
		vec3 tp1(p1.x, p1.y, 0);
		vec3 tp2(p2.x, p2.y, 0);

		data.push_back(tp0);
		data.push_back(tp1);
		data.push_back(tp2);

		triData.push_back(data);

	} */

	vector<vec3> tData;
	//vector<vector<vec3>> triData;

	vec3 p0(10.0, 10.0, 0.0);
	vec3 p1(10.0, -10.0, 0.0);
	vec3 p2(-10.0, 10.0, 0.0);

	tData.push_back(p0);
	tData.push_back(p1);
	tData.push_back(p2);

	triData.push_back(tData);

	// return new Geometry(triData);
	return new Geometry("./work/res/assets/bunny.obj");
	
}

Geometry* RiverHandler::getGeo() {
	return meshDisplay;
}