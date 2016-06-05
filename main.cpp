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
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_geometry.hpp"
#include "geometry.hpp"
#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "spotlight.h"
#include "VoronoiHandler.h"
#include "CImg.h"

using namespace std;
using namespace cgra;
using namespace cimg_library;


int imageSize = 600;
int density = 200;
int radius = 2;
int relaxPasses = 0;

// SimpleGrid pointGrid(20);

Image tex2("./work/res/textures/test_heightmap.png");
// Spotlight

float spotX = 0.0f;
float spotY = 6.0f;
float spotZ = 1.0f;

float spotRotX = 90.0f;
float spotRotY = 0.0f;
float spotRotZ = 0.0f;

float spotDirX = 0.0f;
float spotDirY = 0.0f;
float spotDirZ = 1.0f;


float spot_pos[] = { spotX, spotY, spotZ, 1.0f };
float spot_dir[] = { 0.0, 0.0, 10.0 };
float spot_intensity[] = { 1.0f, 0.8f, 0.2f, 1.0f };
float spot_cutoff = 45.0;
spotlight spot(spot_cutoff, spot_pos, spot_dir, spot_intensity);

float spot_increment = 0.1;
float spotDirIncrement = 1.0;

// Camera rotation
bool rotating = false;
float rotation_increment = 1.0f;
float current_rotation = 0.0f;

//Colours
vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
vec4 red(1.0f, 0.0f, 0.0f, 1.0f);
vec4 green(0.0f, 1.0f, 0.0f, 1.0f);
vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
vec4 purple(1.0f, 0.0f, 1.0f, 1.0f);
vec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
vec4 cyan(0.0f, 1.0f, 1.0f, 1.0f);
vec4 bone(0.99f, 1.0f, 0.0f, 0.98f);

//Teapot colours
vec4 bMetal_diffuse(0.7f, 0.7f, 0.9f, 1.0f);
vec4 bMetal_ambient(0.25f, 0.25f, 0.25f, 1.0f);
vec4 bMetal_specular(1.0f, 1.0f, 1.0f, 1.0f);
float bMetal_shininess = 77.0f;

//Torus colours
vec4 rPlastic_diffuse(0.7f, 0.0f, 0.0f, 1.0f);
vec4 rPlastic_specular(0.7f, 0.6f, 0.6f, 1.0f);
float rPlastic_shiny = 32.0f;

//Spehere colours

vec4 bronze_ambient(0.24725f, 0.1995f, 0.0745f, 1.0f);
vec4 bronze_diffuse(1.0f, 0.8f, 0.22648f, 1.0f);
vec4 bronze_specular(1.01f, 1.0f, 1.0f, 1.0f);
float bronze_shiny = 77.0f;

vec4 black(0.0f, 0.0f, 0.0f, 0.0f);
float matte_test = 20.0f;
float shiny_test = 128.0f;

// Locations
vec4 teapot_position(-5.0, 0.5, -5.0, 1.0);
vec4 box_position(5.0, 2.0, -5.0, 1.0);
vec4 bunny_position(0.0, 0.5, 0.0, 1.0);
vec4 sphere_position(-5.0, 2.0, 5.0, 1.0);
vec4 table_position(0.0, 0.0, 0.0, 1.0);
vec4 torus_position(5.0, 1.0, 5.0, 1.0);




struct material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;

	material() {
		ambient = black;
		diffuse = black;
		specular = black;
		shininess = matte_test;
		emission = black;
	}

	material(vec4 ambient,  vec4 diffuse, vec4 specular, vec4 emission, float shininess) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
		this->emission = emission;
	}
};


material mat_black(black, black, black, black, matte_test);

material shiny_red(black, red, red, black, matte_test);
material shiny_green(black, green, green, black, matte_test);
material shiny_blue(black, blue, white, black, matte_test);
material shiny_purple(black, purple, purple, black, matte_test);
material shiny_yellow(black, yellow, yellow, black, matte_test);
material shiny_cyan(black, cyan, cyan, black, matte_test);

material matte_white(black, white, white, black, matte_test);

material blue_metal(bMetal_ambient, bMetal_diffuse, bMetal_specular, black, bMetal_shininess);
material red_plastic(black, rPlastic_diffuse, rPlastic_specular, black, rPlastic_shiny);
material bronze_metal(bronze_ambient, bronze_diffuse, bronze_specular, black, bronze_shiny);
material bonetest(black, white, white, black, bMetal_shininess);


// Struct for Geometry, position, shading
//
struct geomEntry {

	string name;
	Geometry *geo;
	vec4 position;
	material g_material;

	geomEntry() {
		name = "none";
		geo = nullptr;
		position = vec4(0.0, 0.0, 0.0, 0.0);
		g_material = mat_black;
	}
	geomEntry(string name, Geometry *geo, vec4 position, material mat) {
		this->name = name;
		this->geo = geo;
		this->position = position;
		this->g_material = mat;
	}

};

vector<geomEntry> geometries;


// Geometry
//
Geometry *g_teapot = nullptr;
Geometry *g_table = nullptr;
Geometry *g_sphere = nullptr;
Geometry *g_torus = nullptr;
Geometry *g_box = nullptr;
Geometry *g_bunny = nullptr;





// Window
//
GLFWwindow* g_window;


// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition;
float g_pitch = 0;
float g_yaw = 0;
float g_zoom = 1.0;

// Values and fields to showcase the use of shaders
// Remove when modifying main.cpp for Assignment 3
//
bool g_useShader = false;
GLuint g_boxtexture = 0;
GLuint g_tabletexture = 1;
GLuint g_reftex = 2;

GLuint g_shader = 0;


// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
	if (g_leftMouseDown) {
		g_yaw -= g_mousePosition.x - xpos;
		g_pitch -= g_mousePosition.y - ypos;
	}
	g_mousePosition = vec2(xpos, ypos);
}


// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		g_leftMouseDown = (action == GLFW_PRESS);
	/*
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (g_useShader) {
			g_useShader = false;
			cout << "Using the default OpenGL pipeline" << endl;
		}
		else {
			g_useShader = true;
			cout << "Using a shader" << endl;
		}
	}*/

}


// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
	g_zoom -= yoffset * g_zoom * 0.2;
}


// Keyboard callback
// Called for every key event on since the last glfwPollEvents
//
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	//cout << "Key Callback :: key=" << key << "scancode=" << scancode << "action=" << action << "mods=" << mods << endl;

	if (key == 84 && action == 1) {
		rotating = !rotating;
	}

	if (key == 334 && action == 1) {
		//cout << "+ pressed. Current value: " << spot_cutoff << endl;
		spot_cutoff = spot_cutoff + 1.0;
		//cout << spot_cutoff << endl;
		if (spot_cutoff > 90) spot_cutoff = 90;
		spot.setCutoff(spot_cutoff);
	}

	if (key == 333 && action == 1) {
		//cout << "+ pressed" << endl;
		spot_cutoff = spot_cutoff - 1.0;
		//cout << spot_cutoff << endl;
		if (spot_cutoff < 0) spot_cutoff = 0;
		spot.setCutoff(spot_cutoff);
	}

	if (key == 328 && action == 1) {
		spotY += spot_increment;
		//cout << "spotY: "<< spotY << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}
	if (key == 322 && action == 1) {
		spotY -= spot_increment;
		//cout << "spotY: " << spotY << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}

	if (key == 329 && action == 1) {
		spotX += spot_increment;
		//cout << "spotX: " << spotX << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}

	if (key == 321 && action == 1) {
		spotX -= spot_increment;
		//cout << "spotX: " << spotX << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}

	if (key == 327 && action == 1) {
		spotZ -= spot_increment;
		//cout << "spotZ: " << spotZ << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}

	if (key == 323 && action == 1) {
		spotZ += spot_increment;
		//cout << "spotZ: " << spotZ << endl;
		spot.setPosition(spotX, spotY, spotZ, 1.0f);
	}

	if  (key == 87 && action == 1) {
		spotRotX -= spotDirIncrement;
		//cout << spotRotX << endl;
		if (spotRotX <= -360) spotRotX = 0;
	}

	if (key == 83 && action == 1) {
		spotRotX += spotDirIncrement;
		if (spotRotX >= 360) spotRotX = 0;

	}

	if (key == 65 && action == 1) {
		spotRotY += spotDirIncrement;
		if (spotRotY >= 360) spotRotY = 0;

	}

	if (key == 68 && action == 1) {
		spotRotY -= spotDirIncrement;
		if (spotRotY <= -360) spotRotY = 0;
	}
	if (key == 81 && action == 1) {
		spotRotZ += spotDirIncrement;
		if (spotRotZ >= 360) spotRotZ = 0;
	}

	if (key == 69 && action == 1) {
		spotRotZ -= spotDirIncrement;
		if (spotRotZ <= -360) spotRotZ = 0;
	}

}


// Character callback
// Called for every character input event on since the last glfwPollEvents
//
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;
	// Not needed for this assignment, but useful to have later on
}


// Sets up where and what the light is
// Called once on start up
// 
void initLight() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	//float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float diffintensity[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	// glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);


	glEnable(GL_LIGHT0);
}

/*
void initSpotLight() {



	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
	// glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_intensity);
	glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

	glEnable(GL_LIGHT1);
}
*/

void initPointLight() {
	float direction[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	float diffintensity[] = { 0.3f, 0.2f, 0.2f, 1.0f };
	float specular[] = { 0.3f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);

	glEnable(GL_LIGHT2);

}

void initDirLight() {
	float direction[] = { 0.f, 0.5f, -1.0f, 0.0f };
	float diffintensity[] = { 0.1f, 0.1f, 0.3f, 1.0f };
	float specular[] = { 0.1f, 0.1f, 0.3f, 1.0f };

	glLightfv(GL_LIGHT3, GL_POSITION, direction);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular);

	glEnable(GL_LIGHT3);
}

void initAmbiLight() {
	float ambient[] = { 0.00001f, 0.00001f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT4);
}


// An example of how to load a texure from a hardcoded location
//
void initBoxTexture() {
	Image tex("./work/res/textures/brick.jpg");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_boxtexture); // Generate texture ID
	// glGenTextures(1, 0); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, 1); // Bind it as a 2D texture

	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer());
}

void initTableTexture() {
	// Image tex2("./work/res/textures/test_heightmap.png");

	glActiveTexture(GL_TEXTURE0); // Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glGenTextures(1, &g_tabletexture); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, 2); // Bind it as a 2D texture

											 // Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex2.w, tex2.h, tex2.glFormat(), GL_UNSIGNED_BYTE, tex2.dataPointer());
}

void initReflectionMap() {
	Image left("./work/res/textures/left.jpg");
	Image front("./work/res/textures/front.jpg");
	Image top("./work/res/textures/top.jpg");
	Image bottom("./work/res/textures/bottom.jpg");
	Image right("./work/res/textures/right.jpg");
	Image back("./work/res/textures/back.jpg");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &g_reftex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, g_reftex);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, left.w, left.h, 0, left.glFormat(), GL_UNSIGNED_BYTE, left.dataPointer());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, right.w, right.h, 0, right.glFormat(), GL_UNSIGNED_BYTE, right.dataPointer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, top.w, top.h, 0, top.glFormat(), GL_UNSIGNED_BYTE, top.dataPointer());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, bottom.w, bottom.h, 0, bottom.glFormat(), GL_UNSIGNED_BYTE, bottom.dataPointer());

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, front.w, front.h, 0, front.glFormat(), GL_UNSIGNED_BYTE, front.dataPointer());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, back.w, back.h, 0, back.glFormat(), GL_UNSIGNED_BYTE, back.dataPointer());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	/*
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
	*/
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);


	// glBindTexture(GL_TEXTURE_CUBE_MAP, 3);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

}

// An example of how to load a shader from a hardcoded location
//
void initShader() {
	// To create a shader program we use a helper function
	// We pass it an array of the types of shaders we want to compile
	// and the corrosponding locations for the files of each stage
	g_shader = makeShaderProgramFromFile({GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }, { "./work/res/shaders/shaderDemo.vert", "./work/res/shaders/shaderDemo.frag" });
}


void initGeometry() {

	//g_teapot = new Geometry("./work/res/assets/teapot.obj");
	//g_box = new Geometry("./work/res/assets/box.obj"); // Need a cube
	g_table = new Geometry("./work/res/assets/plane.obj");
	//g_bunny = new Geometry("./work/res/assets/bunny.obj");
	//g_sphere = new Geometry("./work/res/assets/sphere.obj");
	//g_torus = new Geometry("./work/res/assets/torus.obj");

	//geomEntry teapot("teapot", g_teapot, teapot_position, blue_metal);
	//geomEntry box("box", g_box, box_position, matte_white);
	geomEntry table("table", g_table, table_position, matte_white);
	//geomEntry bunny("bunny",g_bunny, bunny_position, bonetest);
	//geomEntry sphere("sphere",g_sphere, sphere_position, bronze_metal);
	//geomEntry torus("torus",g_torus, torus_position, red_plastic);

	// cout << "Teapot shininess: " << teapot.g_material.shininess << endl;

	geometries.push_back(table);
	//geometries.push_back(teapot);
	//geometries.push_back(box);
	//geometries.push_back(sphere);
	//geometries.push_back(torus);
	//geometries.push_back(bunny);
}

// Sets up where the camera is in the scene
// 
void setupCamera(int width, int height) {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -50 * g_zoom);
	glRotatef(g_pitch, 1, 0, 0);
	glRotatef(g_yaw, 0, 1, 0);
}

void setupBoxTex() {

	glEnable(GL_TEXTURE_2D);

	// Use Texture as the color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Set the location for binding the texture
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_TEXTURE); 
	glLoadIdentity(); 
	glScalef(-4.0f, -4.0f, 1.0f); 
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, 1);
}

void setupTableTex() {

	glEnable(GL_TEXTURE_2D);

	// Use Texture as the color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Set the location for binding the texture
	glActiveTexture(GL_TEXTURE0);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity(); 
	//glScalef(-5.0f, -5.0f, 1.0f);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, 2);

}

void setupRefTex() {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 3);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);

	//glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*

	*/



	glBindTexture(GL_TEXTURE_CUBE_MAP, 3);

}


void setupEnvShader() {

	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);
	//glUniform3fv(glGetUniformLocation(g_shader, "cameraPos"), 1, cam.dataPointer());
}

// Draw function
//
void render(int width, int height) {

	// Grey/Blueish background
	// glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.001f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setupCamera(width, height);




	// With shaders (no lighting)
	// Uses the shaders that you bind for the graphics pipeline
	//


	// Texture setup
	//
	// Set the location for binding the texture
	glActiveTexture(GL_TEXTURE0);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, g_tabletexture);

	// Use the shader we made
	glUseProgram(g_shader);

	// Set our sampler (texture0) to use GL_TEXTURE0 as the source
	glUniform1i(glGetUniformLocation(g_shader, "texture0"), 0);


	glUniform1f(glGetUniformLocation(g_shader, "amplitude"), 2.0);
		
	// Render a single square as our geometry
	// You would normally render your geometry here
	/*
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5.0, -5.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0, 5.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0, -5.0, 0.0);
	glEnd();
	glFlush();
	*/
	/*
	glShadeModel(GL_SMOOTH);
	GLfloat colour[] = { 0.5, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, colour);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
	glMaterialfv(GL_FRONT, GL_SPECULAR, colour);
	glMaterialfv(GL_FRONT, GL_SHININESS, colour);
	glMaterialfv(GL_FRONT, GL_EMISSION, colour);
	*/
	// initSpotLight();

	for (geomEntry g : geometries) {
		glPushMatrix; {
			glTranslatef(g.position.x, g.position.y, g.position.z);
			glScalef(10.0, 1.0, 10.0);
			g.geo->renderGeometry();
			glTranslatef(-g.position.x, -g.position.y, -g.position.z);
		}glPopMatrix;
	}

	// Unbind our shader
	glUseProgram(0);


	// Disable flags for cleanup (optional)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
}


// Forward decleration for cleanliness (Ignore)
void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);


//Main program
// 
int main(int argc, char **argv) {
	// Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort(); // Unrecoverable error
	}

	// Get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

	// Create a windowed mode window and its OpenGL context
	g_window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!g_window) {
		cerr << "Error: Could not create GLFW window" << endl;
		abort(); // Unrecoverable error
	}

	// Make the g_window's context is current.
	// If we have multiple windows we will need to switch contexts
	glfwMakeContextCurrent(g_window);



	// Initialize GLEW
	// must be done after making a GL context current (glfwMakeContextCurrent in this case)
	glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}



	// Print out our OpenGL verisions
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



	// Attach input callbacks to g_window
	glfwSetCursorPosCallback(g_window, cursorPosCallback);
	glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
	glfwSetScrollCallback(g_window, scrollCallback);
	glfwSetKeyCallback(g_window, keyCallback);
	glfwSetCharCallback(g_window, charCallback);



	// Enable GL_ARB_debug_output if available. Not nessesary, just helpful
	if (glfwExtensionSupported("GL_ARB_debug_output")) {
		// This allows the error location to be determined from a stacktrace
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the up callback
		glDebugMessageCallbackARB(debugCallbackARB, nullptr);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		cout << "GL_ARB_debug_output callback installed" << endl;
	}
	else {
		cout << "GL_ARB_debug_output not available. No worries." << endl;
	}

	//CImg stuff
	CImg<unsigned char> heightMap("./work/res/textures/test_heightmap.pgm");
	//imageSize = heightMap.width();

	CImg<unsigned char>  pointDisplay(imageSize, imageSize, 1, 3, 0);
	CImg<unsigned char>  pointDisplay2(imageSize, imageSize, 1, 3, 0);

	//pointDisplay.assign("./work/res/textures/test_heightmap.pgm");
	// pointDisplay.channel(3);

	const unsigned char cRed[] = { 255,0,0 };
	const unsigned char cBlue[] = { 0,0,255 };
	const unsigned char cWhite[] = { 255,255,255 };
	const unsigned char cYellow[] = { 255,255,0 };

	VoronoiHandler vHandler(density);

	cout << "Generating Voronoi Polygons..." << endl;
	//vector<vVertexPoint*> pointCloud = vHandler.generatePointSet(density);

	//vector<vTriangle*> triangles = vHandler.generateTriangles(pointCloud);

	//vector<vTriangle*> polygons = vHandler.generateVPolys(pointCloud);

	cout << "Drawing "<< vHandler.getTriangles().size() << " triangles" << endl;

	// Draw Triangle Centers
	for (vTriangle *t : vHandler.getTriangles()) {
		int cx = t->getCenter()->getCoords().x * (imageSize - 1);
		int cy = t->getCenter()->getCoords().y * (imageSize - 1);
		if (t->getCenter()->isBorder()) pointDisplay.draw_circle(cx, cy, radius, cBlue);
		else pointDisplay.draw_circle(cx, cy, radius, cRed);
	}
	
	// Draw Triangle Edges
	/*
	for (vEdge *e : vHandler.getTriEdges()) {
		int p0x = e->v0->getCoords().x * (imageSize - 1);
		int p0y = e->v0->getCoords().y * (imageSize - 1);
		int p1x = e->v1->getCoords().x * (imageSize - 1);
		int p1y = e->v1->getCoords().y * (imageSize - 1);

		int points[4] = { p0x, p0y, p1x, p1y };

		pointDisplay.draw_line(points[0], points[1], points[2], points[3], cWhite);
		if (e->v0->isBorder()) pointDisplay.draw_circle(p0x, p0y, radius, cBlue);
		if (e->v1->isBorder()) pointDisplay.draw_circle(p1x, p1y, radius, cBlue);
	}
	*/
	cout << "Drawing " << vHandler.getPolygons().size() << " polygons" << endl;

	// Draw Polygon Edges
	for (vEdge *e : vHandler.getPolyEdges()) {
		int p0x = e->v0->getCoords().x * (imageSize - 1);
		int p0y = e->v0->getCoords().y * (imageSize - 1);
		int p1x = e->v1->getCoords().x * (imageSize - 1);
		int p1y = e->v1->getCoords().y * (imageSize - 1);

		int points[4] = { p0x, p0y, p1x, p1y };

		pointDisplay.draw_line(points[0], points[1], points[2], points[3], cYellow);
		// if (e->v0->isBorder()) pointDisplay.draw_circle(p0x, p0y, radius, cBlue);
		// if (e->v1->isBorder()) pointDisplay.draw_circle(p1x, p1y, radius, cBlue);
	}

	// CImgDisplay draw_disp(pointDisplay, "Raw Mesh"), draw_disp2(pointDisplay2, "Relaxed Mesh");

	CImgDisplay draw_disp(pointDisplay, "Raw Mesh");


	initBoxTexture();
	initTableTexture();
	initReflectionMap();
	initShader();
	initGeometry();

	spot_cutoff = 45.0;
	spotX = 0.0f;
	spotY = 6.0f;
	spotZ = 1.0f;

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(g_window)) {

		// Make sure we draw to the WHOLE window
		int width, height;
		glfwGetFramebufferSize(g_window, &width, &height);

		// Main Render
		render(width, height);
		draw_disp.wait();
		//draw_disp2.wait();
		// Swap front and back buffers
		glfwSwapBuffers(g_window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
}






//-------------------------------------------------------------
// Fancy debug stuff
//-------------------------------------------------------------

// function to translate source to string
string getStringForSource(GLenum source) {

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return("API");
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return("Window System");
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return("Shader Compiler");
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return("Third Party");
	case GL_DEBUG_SOURCE_APPLICATION:
		return("Application");
	case GL_DEBUG_SOURCE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// function to translate severity to string
string getStringForSeverity(GLenum severity) {

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return("HIGH!");
	case GL_DEBUG_SEVERITY_MEDIUM:
		return("Medium");
	case GL_DEBUG_SEVERITY_LOW:
		return("Low");
	default:
		return("n/a");
	}
}

// function to translate type to string
string getStringForType(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return("Error");
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return("Deprecated Behaviour");
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return("Undefined Behaviour");
	case GL_DEBUG_TYPE_PORTABILITY:
		return("Portability Issue");
	case GL_DEBUG_TYPE_PERFORMANCE:
		return("Performance Issue");
	case GL_DEBUG_TYPE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// actually define the function
void APIENTRY debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, GLvoid*) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) return;

	cerr << endl; // extra space

	cerr << "Type: " <<
		getStringForType(type) << "; Source: " <<
		getStringForSource(source) << "; ID: " << id << "; Severity: " <<
		getStringForSeverity(severity) << endl;

	cerr << message << endl;

	if (type == GL_DEBUG_TYPE_ERROR_ARB) throw runtime_error("");
}