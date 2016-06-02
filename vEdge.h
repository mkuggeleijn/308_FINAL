#pragma once

#ifndef VEDGE_H
#define VEDGE_H

#include <vector>

#include "vVertexPoint.h"
#include "vTriangle.h"

using namespace std;
using namespace cgra;

class vEdge;
class vTriangle;
class vVertexPoint;

class vEdge {

public:
	vector<vTriangle*> polys;
	vector<vEdge*>neighbours;
	vVertexPoint* v0;
	vVertexPoint* v1;

	vEdge(vVertexPoint*, vVertexPoint*, vTriangle*);

	static vEdge* checkDuplicate(vVertexPoint*, vVertexPoint*, vTriangle*);



};

#endif VEDGE_H