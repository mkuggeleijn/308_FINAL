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
	}
}

Geometry::Geometry(std::vector<std::vector<cgra::vec3>> triangles) {
	cout << "Building geometry from triangle data" << endl;
	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0, 0, 0));
	m_uvs.push_back(vec2(0, 0));
	m_normals.push_back(vec3(0, 0, 1));

	for (std::vector<cgra::vec3> t : triangles) {
		vertex v0, v1, v2;

		m_points.push_back(t.at(0));
		v0.p = m_points.size()-1;

		m_points.push_back(t.at(1));
		v1.p = m_points.size() - 1;

		m_points.push_back(t.at(2));
		v2.p = m_points.size() - 1;

		//cout << "Adding " << t.at(0) << ", " << t.at(1) << "," << t.at(2) << endl;

		triangle tri;

		tri.v[0] = v0;
		tri.v[1] = v1;
		tri.v[2] = v2;

		m_triangles.push_back(tri);

	}
	

	cout << "Made geo from data." << endl;
	cout << m_points.size() - 1 << " points" << endl;
	cout << m_uvs.size() - 1 << " uv coords" << endl;
	cout << m_normals.size() - 1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;

	if (m_normals.size() <= 1) createNormals();

	cout << "Generated " <<m_normals.size()-1 << " normals." << endl;

	if (m_triangles.size() > 0) {
		createDisplayListPoly();
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

					// Assignment code (assumes you have all of v/vt/vn for each vertex)
					if(m_points.size() > 1 && m_uvs.size() > 1 && m_normals.size() >1){
						objLine >> v.p;		// Scan in position index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.t;		// Scan in uv (texture coord) index
						objLine.ignore(1);	// Ignore the '/' character
						objLine >> v.n;		// Scan in normal index
						verts.push_back(v);
					} else if(m_points.size() > 1 && m_uvs.size() <= 1 && m_normals.size() >1){
						objLine >> v.p;	// Scan in position index
						objLine.ignore(2);	// Ignore the '//' characters
						objLine >> v.n;	// Scan in normal index
						verts.push_back(v);
					} else if(m_points.size() > 1 && m_uvs.size() <= 1 && m_normals.size() <=1){
						objLine >> v.p;	// Scan in position index
						verts.push_back(v);
					}
				}

				// IFF we have 3 verticies, construct a triangle face
				if(verts.size() >= 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				} //else {cout << filename << "  : verts.size: " << verts.size() << endl;}
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
	// YOUR CODE GOES HERE

	for(unsigned int i=0;i<m_triangles.size();i++){
		m_triangles[i].v[0].n = m_triangles[i].v[0].p;
		m_triangles[i].v[1].n = m_triangles[i].v[1].p;
		m_triangles[i].v[2].n = m_triangles[i].v[2].p;
	}

	// makes a 2d vector, first index is point number in m_points
	// and second index is face number in m_triangles
	vector< vector<int> > shared_points(m_points.size()); // [point index][face index]
	for(unsigned int i=0;i<m_triangles.size();i++){
		for(int j=0; j<3; j++){
			triangle tri = m_triangles[i];
			int i_point = tri.v[j].p;
			int i_face = i;
			shared_points[i_point].push_back(i_face);
		}
	}


	for(int i=1; i<shared_points.size(); i++){
		// add then normalize all faces that share particular point
		triangle tri;
		vec3 vertex_normal(0.0,0.0,0.0);
		int count = 0; // count how many faces share this face -- could remove and use shared_points[i].size()
		for(int j=0; j<shared_points[i].size(); j++){
			tri = m_triangles[shared_points[i][j]];
			vec3 u = m_points[tri.v[0].p] - m_points[tri.v[1].p];
			vec3 v = m_points[tri.v[0].p] - m_points[tri.v[2].p];
			vertex_normal += cross(u,v);
			count++;

		}
		if(count > 1){
			//vertex_normal /= float(count);
			vertex_normal = normalize(vertex_normal);
		}
		cout << "Generated normal: " << vertex_normal << endl;
		m_normals.push_back(vertex_normal);
	}

}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

	// YOUR CODE GOES HERE
	// ...
	glBegin(GL_TRIANGLES);
	for(unsigned int i=0; i<m_triangles.size();i++){
		triangle tri = m_triangles[i];

		//look up p1, p2, p3 vectors
		for(unsigned int j=0;j<3;j++){

			if(m_normals.size() > 1){
				vec3 vn = m_normals[tri.v[j].n];
				GLfloat nx,ny,nz;
				nx = vn.x;
				ny = vn.y;
				nz = vn.z;
				glNormal3f(nx,ny,nz);
			}

			if(m_uvs.size() > 1){
				vec2 vt = m_uvs[tri.v[j].t];
				GLfloat tx,ty;
				tx = vt.x; //should be u,v instead of xy for texture co-ords
				ty = vt.y; //should be u,v instead of xy for texture co-ords
				glTexCoord2f(tx,ty);
			}

			vec3 vp = m_points[tri.v[j].p];
			GLfloat px,py,pz;
			px = vp.x;
			py = vp.y;
			pz = vp.z;
			glVertex3f(px,py,pz);
		}
	}
	glEnd();

	/*
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 0, 1);
	glVertex3f(10.0, 10.0, 0.0);
	glVertex3f(10.0, -10.0, 0.0);
	glVertex3f(-10.0, 10.0, 0.0);
	glEnd();
	*/
	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}



void Geometry::renderGeometry() {

		//glShadeModel(GL_SMOOTH);
		glCallList(m_displayListPoly);
}

