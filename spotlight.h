#pragma once

#include <cmath>
#include <cstdlib>

#include "cgra_geometry.hpp"
#include "cgra_math.hpp"
#include "opengl.hpp"

class spotlight {

private:
	// Light values
	float cutoff;
	float* position;
	float* direction;
	float* intensity;

	// Geo values
	float sphereRadius = 0.15;
	float directionRadius = 0.05;
	float directionHeight = 0.15;
	float cutoffRadius = 0.07;
	float cutoffHeight = 0.15;
	float stemHeight = 0.75;
	float stemRadius = 0.01;

	// Helpers
	cgra::vec3 zAxis = cgra::vec3(0.0f, 0.0f, 1.0f);
	cgra::vec3 yAxis = cgra::vec3(0.0f, 1.0f, 0.0f);
	cgra::vec3 xAxis = cgra::vec3(1.0f, 0.0f, 0.0f);
	cgra::vec3 rotAxis;
	cgra::vec3 spotDir;
	double spotRot;

	// Spotlight geo etc.
	void drawSpotLightSource();
	void drawSpotGeo();


public:
	// Constructors/Destructor
	spotlight();
	spotlight(float, float*, float*, float*);
	~spotlight();
	
	// Set/Get
	float getCutoff();
	void setCutoff(float);

	float* getPosition();
	void setPosition(float*);
	void setPosition(float, float, float, float);

	float* getDirection();
	void setDirection(float*);
	void setDirection(float, float, float);

	float* getIntensity();
	void setIntensity(float*);

	void updateSpotlight(float, float*, float*, float*);

	// Draw Spotlight
	void drawSpotlight(float, float, float);


};