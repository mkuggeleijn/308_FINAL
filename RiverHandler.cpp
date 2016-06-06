#include "RiverHandler.h"


RiverHandler::RiverHandler() {
	
	this -> graph = new VoronoiHandler(density);
	graph -> setScreenCoords(imageSize);
	this -> riverPoints = sampleImage(heightMap);
	this -> riverSources = findSourceCandidates(riverPoints);


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

// Sample intensity of heightmap and add data to polygon graph
vector<vVertexPoint*> RiverHandler::sampleImage(Image heightMap) {
	vector<vVertexPoint*> riverPoints;

	for (vVertexPoint* p : graph->getPolyVertices()) {
		int screenX = p->getCoords().x * (imageSize - 1);
		int screenY = p->getCoords().y * (imageSize - 1);

		p->screenCoords.x = screenX;
		p->screenCoords.y = screenY;

		p->setZValue(heightMap.getIntensity(screenX, screenY));
		riverPoints.push_back(p);
	}
	sort(riverPoints.begin(), riverPoints.end(), sortByZ());
	return riverPoints;
}

vector<vVertexPoint*>RiverHandler::findSourceCandidates(vector<vVertexPoint*> riverPoints) {

	vector<vVertexPoint*>riverSources;

	int cutPercent = cutoffPercent;
	int riverCount = 0;

	//std::random_device rd1;
	//std::mt19937 gen1(rd1());

	while (riverSources.size() != numberOfRivers) {
		int	riverLimit = riverPoints.size() * (cutPercent / 100);
		// std::uniform_real_distribution<> dis(0, riverLimit -1);
		for (int x = 0; x < riverLimit; x++) {
			// int n = dis(gen1);
			int n = rand() % (riverLimit - 1);
			if(find(riverSources.begin(), riverSources.end(), riverPoints.at(n)) == riverSources.end())
				riverSources.push_back(riverPoints.at(n));
		}
		if (riverSources.size() != numberOfRivers) cutPercent += 10;
		if (cutPercent > 100) break;
	}

	return riverSources;
}

vector<vector<RiverPoint*>> RiverHandler::makeRivers(int numberOfRivers, vector<vVertexPoint*> riverSources) {
	vector<vVertexPoint*> starts;
	vector<vector<RiverPoint*>> rivers;

	//std::random_device rd1;
	//std::mt19937 gen1(rd1());
	//std::uniform_real_distribution<> dis(0, riverSources.size()-1);

	for (int x = 0; x < numberOfRivers; x++) {
		//int n = dis(gen1);
		int n = rand() % (riverSources.size() - 1);
		if (find(starts.begin(), starts.end(), riverSources.at(n)) == starts.end()) {
			starts.push_back(riverSources.at(n));
			rivers.push_back(makeRiverPath(riverSources.at(n)));
		}
	}

	return rivers;
}

vector<RiverPoint*> RiverHandler::makeRiverPath(vVertexPoint* source) {

	vector<RiverPoint*> river;

	RiverPoint *riverHead = new RiverPoint(source);
	riverHead = getNextRiverPoint(riverHead);

	RiverPoint *r = riverHead;
	while (!r->isExit()) {
		river.push_back(r);
		r = r->getDownstream();
	}
	river.push_back(r);

	return river;
}

RiverPoint* RiverHandler::getNextRiverPoint(RiverPoint *parent) {
	vVertexPoint *location = parent->getLocation();
	
	vVertexPoint *lowest = location;
	for (vEdge* e : location->getEdges()) {
		vVertexPoint *end;
		if (e->v0 == location) end = e->v1;
		else end = e->v0;
		if (end->getZValue() < lowest->getZValue())
			lowest = end;
	}
	if (lowest == location) lowest = dealWithLocalMin(parent);

	RiverPoint *child = new RiverPoint(lowest);
	int waterAdd = parent->getWater() + ((distance(child->getLocation()->getCoords(), parent->getLocation()->getCoords())) * 100);
	child->setWater(waterAdd);

	if (lowest->isBorder()) {
		child->setExit(true);
		return child;
	}

	return(getNextRiverPoint(child));

}

vVertexPoint* RiverHandler::dealWithLocalMin(RiverPoint* parent) {
	vVertexPoint *location = parent->getLocation();
	vVertexPoint *lowest;
	for (int x = 0; x < location->getEdges().size(); x++) {
		vEdge *e = location->getEdges().at(x);
		vVertexPoint *end;
		if (e->v0 == location) end = e->v1;
		else end = e->v0;
		if (x = 0) lowest = end;
		else if (end->getZValue() < lowest->getZValue()) lowest = end;
	}
	return lowest;
}

void RiverHandler::drawAll() {
	//CImg stuff, needed for testing
	const unsigned char cRed[] = { 255,0,0 };
	const unsigned char cBlue[] = { 0,0,255 };
	const unsigned char cWhite[] = { 255,255,255 };
	const unsigned char cGrey[] = { 127,127,127 };
	const unsigned char cYellow[] = { 255,255,0 };

	CImg<unsigned char>  pointDisplay(imageSize, imageSize, 1, 3, 0);

	int radius = 2;

	//cout << "Found " << graph->getTriangles().size() << " triangles, with " << graph->getTriEdges().size() << " edges." << endl;
	//cout << "Found " << graph->getPolygons().size() << " polygons, with " << graph->getPolyEdges().size() << " edges." << endl;

	drawEdges(graph->getPolyEdges(), &pointDisplay, cGrey);
	drawPoints(graph->getPolyVertices(), &pointDisplay, cWhite,radius);
	//drawPolygons(graph.getTriangles(), &pointDisplay, cGrey, cWhite, radius);
	drawRivers(rivers, &pointDisplay, cRed,radius);

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
void RiverHandler::drawPoints(vector<vVertexPoint*> points, CImg<unsigned char> *pointDisplay, const unsigned char color[], int radius) {
	for (vVertexPoint* p : points) {
		pointDisplay->draw_circle(p->screenCoords.x, p->screenCoords.y, radius, color);
	}

}
void RiverHandler::drawRivers(vector<vector<RiverPoint*>> riverSet, CImg<unsigned char> *pointDisplay, const unsigned char color[], int radius) {
	for (vector<RiverPoint*> river : riverSet) {
		RiverPoint *r = river.at(0);
		while (!r->isExit()){
			int p0x = r->getLocation()->screenCoords.x;
			int p0y = r->getLocation()->screenCoords.y;
			int p1x = r->getDownstream()->getLocation()->screenCoords.x;
			int p1y = r->getDownstream()->getLocation()->screenCoords.y;

			int points[4] = { p0x, p0y, p1x, p1y };
			pointDisplay->draw_line(points[0], points[1], points[2], points[3], color);
			pointDisplay->draw_circle(p0x, p0y, radius, color);
			pointDisplay->draw_circle(p1x, p1y, radius, color);
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