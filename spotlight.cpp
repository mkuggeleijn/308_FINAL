#pragma once
#include "spotlight.h"

using namespace std;
using namespace cgra;

//Constructors/Destructor
spotlight::spotlight() {

	float position[] = { 0.0f, 6.0f, 1.0f, 1.0f };
	float direction[] = { 0.0f, 0.0f, 1.0f };
	float intensity[] = { 1.0f, 0.8f, 0.2f, 1.0f };
	float cutoff = 45.0f;

	setCutoff(cutoff);
	setDirection(direction);
	this->position = position;
	this->intensity = intensity;



}
spotlight::spotlight(float cutoff, float* position, float* direction, float* intensity) {

	setCutoff(cutoff);
	setDirection(direction);
	this->position = position;
	this->intensity = intensity;

}

spotlight::~spotlight() {}

// Set/Get
float spotlight::getCutoff() {
	return this->cutoff;
}
void spotlight::setCutoff(float cutoff) {
	this->cutoff = cutoff;
	this->cutoffRadius = cutoffHeight*tan(radians(cutoff/2.0f));
	cout << "cutoff cone radius = " << cutoffRadius << endl;

}

float* spotlight::getPosition() {
	return this->position;

}
void spotlight::setPosition(float* position) {
	this->position = position;
}

void spotlight::setPosition(float x, float y, float z, float w) {

	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
	this->position[3] = w;

	cout << "New Spot position: " << "("<<position[0] << ", " << position[1] << ", " << position[2] << ", " << position[3]<<")"<<endl;
}

float* spotlight::getDirection() {
	return this->direction;
}
void spotlight::setDirection(float* direction) {
	this->direction = direction;
	spotDir = vec3(direction[0], direction[1], direction[2]);

	// angle between spot direction and zAxis (forward direction)
	rotAxis = cross(zAxis, spotDir);
	// axis of rotation
	spotRot = degrees(acos(dot(spotDir, zAxis)));
}

void spotlight::setDirection(float x, float y, float z) {
	this->direction[0] = x;
	this->direction[0] = y;
	this->direction[0] = z;
	spotDir = vec3(direction[0], direction[1], direction[2]);

	// angle between spot direction and zAxis (forward direction)
	rotAxis = cross(zAxis, spotDir);
	// axis of rotation
	spotRot = degrees(acos(dot(spotDir, zAxis)));
}

float* spotlight::getIntensity() {
	return this->intensity;
}
void spotlight::setIntensity(float*) {
	this->intensity = intensity;
}

void spotlight::updateSpotlight(float cutoff, float* position, float* direction, float* intensity) {

	setCutoff(cutoff);
	setDirection(direction);
	this->position = position;
	this->intensity = intensity;

}

// Draw Spotlight

void spotlight::drawSpotLightSource() {
	
	float default_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.5f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, intensity);
	glLightfv(GL_LIGHT1, GL_POSITION, default_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);

	glEnable(GL_LIGHT1);

}

void spotlight::drawSpotGeo() {

	glPushMatrix(); {
		cgraSphere(sphereRadius);

		glRotatef(spotRot, rotAxis[0], rotAxis[1], rotAxis[2]);
		glTranslatef(0, 0, sphereRadius);
		glTranslatef(0, 0, cutoffHeight);
		glRotatef(180, 1, 0,0);



		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cgraCone(cutoffRadius, cutoffHeight);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glRotatef(-180, 1, 0, 0);
		glTranslatef(0, 0, -cutoffHeight);


		cgraCylinder(stemRadius, stemRadius, stemHeight);
		glTranslatef(0, 0, stemHeight);
		cgraCone(directionRadius, directionHeight);
		/*
		glTranslatef(0, 0, -stemHeight);
		glTranslatef(0, 0, -sphereRadius);
		glRotatef(-spotRot, rotAxis[0], rotAxis[1], rotAxis[2]);
		glTranslatef(-position[0], -position[1], -position[2]);*/

	}glPopMatrix();
}



void spotlight::drawSpotlight(float spotRotX, float spotRotY, float spotRotZ) {
	glPushMatrix; {
		glTranslatef(position[0], position[1], position[2]);
		
		// cout << "(" << spotRotX << ","<< spotRotY << "," << spotRotZ << ")" << endl;

		glRotatef(spotRotZ, 0.0, 0.0, 1.0);
		glRotatef(spotRotY, 0.0, 1.0, 0.0);
		glRotatef(spotRotX, 1.0, 0.0, 0.0);
		
		
		
		

		drawSpotLightSource();
		glDisable(GL_LIGHTING);
		drawSpotGeo();
		glEnable(GL_LIGHTING);
		//glTranslatef(position[0], position[1], position[2]);
	} glPopMatrix();
}