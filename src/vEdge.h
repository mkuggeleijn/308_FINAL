#pragma once

#include "vVertexPoint.h"

class vEdge {
public:
	vVertexPoint* v1;
	vVertexPoint* v2;

	vVertexPoint* direction;

	vVertexPoint* left;
	vVertexPoint* right;

	double f;
	double g;

	vEdge* neighbour;

	vEdge(vVertexPoint*, vVertexPoint*, vVertexPoint*);

};