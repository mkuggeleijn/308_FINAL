#include "vEdge.h"


vEdge::vEdge(vVertexPoint *start, vVertexPoint *left, vVertexPoint *right) {
	this->v1 = start;
	this->v2 = 0;

	this->left = left;
	this->right = right;
	
	this->direction = 0;
}