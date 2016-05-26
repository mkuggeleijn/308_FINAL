//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

// #include "cgra_cow.hpp"
#include "cgra_math.hpp"
#include "geometry.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;


Geometry::Geometry(string filename) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
		createDisplayListWire();
	}
}


Geometry::~Geometry() { }


void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));

	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					//-------------------------------------------------------------
					// [Assignment 1] :
					// Modify the following to parse the bunny.obj. It has no uv
					// coordinates so each vertex for each face is in the format
					// v//vn instead of the usual v/vt/vn.
					//
					// Modify the following to parse the dragon.obj. It has no
					// normals or uv coordinates so the format for each vertex is
					// v instead of v/vt/vn or v//vn.
					//
					// Hint : Check if there is more than one uv or normal in
					// the uv or normal vector and then parse appropriately.
					//-------------------------------------------------------------
					
					objLine >> v.p;					// Scan in position index

					if (m_uvs.size() <= 1 && m_normals.size() <= 1){ 	// v only, do nothing further (eg; dragon.obj)
					} else {
						if (m_uvs.size() <= 1) {			// v//vn (eg; bunny.obj)
							// cout << "No vt data" << endl;
							objLine.ignore(1);			// Ignore the '/' character
							objLine.ignore(1);			// Ignore the next '/' character
							objLine >> v.n;				// Scan in normal index

						} else {					// v/vt/vn (eg; teapot.obj)
							objLine.ignore(1);			// Ignore the '/' character
							objLine >> v.t;				// Scan in uv (texture coord) index
							objLine.ignore(1);			// Ignore the '/' character
							objLine >> v.n;				// Scan in normal index
							}
					}

					verts.push_back(v);
				}

				// If we have 3 vertices, construct a triangle
				if(verts.size() >= 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
					tri.v[0].n = 0;
					tri.v[1].n = 0;
					tri.v[2].n = 0;

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;

	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
}

//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to populate the normals for 
// the model currently loaded. Compute per face normals
// first and get that working before moving onto calculating
// per vertex normals.
//-------------------------------------------------------------

void Geometry::createNormals() {
	
	// Remember; a Normal is the cross product of two vectors

	cout << "Creating Normals..." << endl;
	cout << "Triangles: " << m_triangles.size() << endl;

	// First calculate face normals for each triangle

	for (triangle &tri : m_triangles) {

		// vector from points;
		// end point - start point = vector

		vec3 v1 = m_points[tri.v[1].p] - m_points[tri.v[0].p];
		vec3 v2 = m_points[tri.v[2].p] - m_points[tri.v[0].p];
	
		vec3 fn = cross(v1,v2);
		// fn = normalize(fn);

		for(vertex &vert : tri.v){
			if (vert.n == 0) {
				m_normals.push_back(fn);
				vert.n = m_normals.size()-1;
			} else {
				m_normals[vert.n] = normalize(m_normals[vert.n] + fn);		
			}

		}


	}

	
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as polygon model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	for (int x = 0; x < m_triangles.size(); x++) {
		
		vertex vert1 = m_triangles[x].v[0];
		vertex vert2 = m_triangles[x].v[1];
		vertex vert3 = m_triangles[x].v[2];

		glNormal3f(m_normals[vert1.n].x, m_normals[vert1.n].y, m_normals[vert1.n].z);
		if (m_uvs.size() > 1) { 
			//cout << m_uvs[vert1.t] << endl;
			glTexCoord2f(m_uvs[vert1.t].x, m_uvs[vert1.t].y); 
		}
		glVertex3f(m_points[vert1.p].x,m_points[vert1.p].y,m_points[vert1.p].z);

		glNormal3f(m_normals[vert2.n].x, m_normals[vert2.n].y, m_normals[vert2.n].z);
		if (m_uvs.size() > 1) {
			//cout << m_uvs[vert2.t]<< endl;
			glTexCoord2f(m_uvs[vert2.t].x, m_uvs[vert2.t].y); 
		}
		glVertex3f(m_points[vert2.p].x,m_points[vert2.p].y,m_points[vert2.p].z);

		glNormal3f(m_normals[vert3.n].x, m_normals[vert3.n].y, m_normals[vert3.n].z);
		if (m_uvs.size() > 1) { 
			//cout << m_uvs[vert3.t] << endl;
			glTexCoord2f(m_uvs[vert3.t].x, m_uvs[vert3.t].y); 
		}
		glVertex3f(m_points[vert3.p].x,m_points[vert3.p].y,m_points[vert3.p].z);
	}
	glEnd();
	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListWire() {
	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);

	// Create a new list
	cout << "Creating Wire Geometry" << endl;
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE);

	for (int x = 0; x < m_triangles.size(); x++) {

		glBegin(GL_LINE_LOOP);
		
		vertex vert1 = m_triangles[x].v[0];
		vertex vert2 = m_triangles[x].v[1];
		vertex vert3 = m_triangles[x].v[2];

		glNormal3f(m_normals[vert1.n].x, m_normals[vert1.n].y, m_normals[vert1.n].z);
		glVertex3f(m_points[vert1.p].x,m_points[vert1.p].y,m_points[vert1.p].z);

		glNormal3f(m_normals[vert2.n].x, m_normals[vert2.n].y, m_normals[vert2.n].z);
		glVertex3f(m_points[vert2.p].x,m_points[vert2.p].y,m_points[vert2.p].z);

		glNormal3f(m_normals[vert3.n].x, m_normals[vert3.n].y, m_normals[vert3.n].z);
		glVertex3f(m_points[vert3.p].x,m_points[vert3.p].y,m_points[vert3.p].z);

		glEnd();
	}

	glEndList();
	cout << "Finished creating Wire Geometry" << endl;
}


void Geometry::renderGeometry() {
	/*
	if (m_wireFrameOn) {
		glShadeModel(GL_SMOOTH);
		glCallList(m_displayListWire);

	} else {
		glShadeModel(GL_SMOOTH);
		glCallList(m_displayListPoly);

	}
	*/
	glShadeModel(GL_SMOOTH);
	glCallList(m_displayListPoly);

}


void Geometry::toggleWireFrame() {
	m_wireFrameOn = !m_wireFrameOn;
}

