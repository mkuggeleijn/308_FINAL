#include "RiverHandler.h"


RiverHandler::RiverHandler() {
	
	this -> graph = new VoronoiHandler(density);
	graph -> sampleImage(imageSize, heightMap);
	this -> riverSources = findSourceCandidates(graph->getPolyVertices());
	cout << "Found " << riverSources.size() << " river source candidates." << endl;
	cout << "Making " << numberOfRivers << " rivers..." << endl;
	this->rivers = makeRivers(numberOfRivers, riverSources);
	//cout << "Found " << rivers.size() << " rivers." << endl;
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

struct sortByZ {
	bool operator()(vVertexPoint *p1, vVertexPoint *p2) const {
		return p1->getZValue() > p2->getZValue();
	}
};



// Get certain amount of highest points
vector<vVertexPoint*>RiverHandler::findSourceCandidates(vector<vVertexPoint*> riverPoints) {

	sort(riverPoints.begin(), riverPoints.end(), sortByZ());

	vector<vVertexPoint*>riverSources;

	int riverCutOff = riverPoints.size() * (cutoffPercent / 100.0);
	cout << "riverCutOff: " << riverCutOff;
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

	//std::random_device rd1;
	//std::mt19937 gen1(rd1());
	//std::uniform_real_distribution<> dis(0, riverSources.size-1);

	for (int x = 0; x < numberOfRivers; x++) {
		//int n = dis(gen1);
		int n = rand() % (riverSources.size() - 1);
		vVertexPoint *source = riverSources.at(n);
		while (source->isRiver()) {
			cout << "In the river loop" << endl;
			//n = dis(gen1);
			int n = rand() % (riverSources.size() - 1);
			vVertexPoint *source = riverSources.at(n);
		}
		vector<vVertexPoint*> newRiver = makeRiverPath(source);
		rivers.push_back(newRiver);
	}
	cout << "Made " << rivers.size() << " rivers" << endl;
	return rivers;
}

// Generate a single river
vector<vVertexPoint*> RiverHandler::makeRiverPath(vVertexPoint* source) {

	vector<vVertexPoint*> river;
	river.push_back(source);
	source->setDownstream(getNextRiverPoint(source, &river));
	return river;
}

// Find the next point of the river
vVertexPoint* RiverHandler::getNextRiverPoint(vVertexPoint *parent, vector<vVertexPoint*> *river) {

	vector<vVertexPoint*> neighbours = parent->getNeighbours();
	sort(neighbours.begin(), neighbours.end(), sortByZ());
	vVertexPoint *next = neighbours.at(0);
	if (neighbours.size() > 1) {
		for (int x = 1; x < neighbours.size(); x++) {
			if (find(river->begin(), river->end(), next) == river->end()) {
				next = neighbours.at(x);
				break;
			}
		}
	}
	// if (next->getZValue() > parent->getZValue()) local minimum, tricky
	
	float water = distance(next->getCoords(), parent->getCoords());
	river->push_back(next);

	// If we're merging two rivers
	if (next->isRiver() && (find(river->begin(), river->end(), next) == river->end())) {
		next->updateFlow(water);
		return next;
	}

	next->setRiver(true);
	next->setWater(water);

	if (next->isBorder()) return next;

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
	pointDisplay.assign("./work/res/textures/simplebump.pgm");
	int radius = 2;

	cout << "Found " << graph->getTriangles().size() << " triangles, with " << graph->getTriEdges().size() << " edges." << endl;
	//cout << "Found " << graph->getPolygons().size() << " polygons, with " << graph->getPolyEdges().size() << " edges." << endl;

	drawEdges(graph->getTriEdges(), &pointDisplay, cGrey);
	drawEdges(graph->getPolyEdges(), &pointDisplay, cDarkGrey);
	drawPoints(graph->getPolyVertices(), &pointDisplay, cBlack ,cGrey, radius);
	//drawPolygons(graph.getTriangles(), &pointDisplay, cGrey, cWhite, radius);
	drawRivers(rivers, &pointDisplay, cWhite, cWhite,radius);

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
			cout << "Riverpoint " << r << " at " << r->screenCoords << endl;
			int p0x = r->screenCoords.x;
			int p0y = r->screenCoords.y;
			pointDisplay->draw_circle(p0x, p0y, radius, nodeColor);
			if (!r->isBorder()) {
				int p1x = r->getDownstream()->screenCoords.x;
				int p1y = r->getDownstream()->screenCoords.y;
				int points[4] = { p0x, p0y, p1x, p1y };
				cout << "Riverline: " << points << endl;
				pointDisplay->draw_line(points[0], points[1], points[2], points[3], lineColor);
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