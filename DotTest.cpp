#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_math.hpp"
#include "opengl.hpp"
#include "VoronoiHandler.h"
#include "CImg.h"
#include "geometry.hpp"

using namespace std;
using namespace cgra;
using namespace cimg_library;

//Plane geometry
Geometry *g_plane = nullptr;



VoronoiHandler vHandler;
int imageSize = 800;
int density = 100;
int radius = 2;

// Window
//
GLFWwindow* g_window;

// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.001;
float g_zfar = 1000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
bool g_rightMouseDown = false;

vec2 g_mousePosition;
float g_pitch = 0;
float g_yaw = 0;
float g_zoom = 1.0;
float g_rot = 1.0;

const unsigned char red[] = { 255,0,0 };
CImg<unsigned char>  pointDisplay(imageSize, imageSize, 1, 3, 0);


// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;

	if (g_leftMouseDown) {
		g_yaw -= g_mousePosition.x - xpos;
		g_pitch -= g_mousePosition.y - ypos;
	}

	if (g_rightMouseDown) {		
		// Move camera
	}

	g_mousePosition = vec2(xpos, ypos);

}

// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	// send messages to the GUI manually
	// SimpleGUI::mouseButtonCallback(win, button, action, mods);

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		g_leftMouseDown = (action == GLFW_PRESS);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		g_rightMouseDown = (action == GLFW_PRESS);
	}

}

// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
	g_zoom -= yoffset * g_zoom * 0.2;
}


// Sets up where and what the light is
// 
void setupLight() {
	// No transform for the light
	// makes it move realitive to camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vec4 direction(0.0, 0.0, 1.0, 0.0);
	vec4 diffuse(0.7, 0.7, 0.7, 1.0);
	vec4 ambient(0.2, 0.2, 0.2, 1.0);

	glLightfv(GL_LIGHT0, GL_POSITION, direction.dataPointer());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.dataPointer());
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.dataPointer());

	glEnable(GL_LIGHT0);
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

	glTranslatef(0, 0, -5 * g_zoom);
	// glTranslatef(arcCenter.x, -arcCenter.y, 0);

	glRotatef(g_pitch, 1, 0, 0);
	glRotatef(g_yaw, 0, 1, 0);

}


// Render one frame to the current window given width and height
//
void render(int width, int height) {

	// Set viewport to be the whole window
	glViewport(0, 0, width, height);

	setupLight();

	// Setup camera
	setupCamera(width, height);

	// Grey/Blueish background
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);


	// Render geometry
	glColor3f(1.0,0.0,0.0);
	g_plane->renderGeometry();
	// plane goes here

	// Disable flags for cleanup (optional)
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
}


// Forward decleration for cleanliness (Ignore)
//void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);


int main() {

	g_plane = new Geometry("./work/res/assets/plane.obj");

	vector<vVertexPoint> pointCloud = vHandler.generatePointSet(density);

	//pointCloud = vHandler.generatePointSet(density);
	// vHandler.generateVPolys(&pointCloud);

	cout << "Drawing Points" << endl;

	for (vVertexPoint point : pointCloud) {

		cout << "point coords: "<< point.getCoords() << endl;
		int screenX = point.getCoords().x * imageSize;
		int screenY = point.getCoords().y * imageSize;
		cout << "screen coords: (" << screenX << "," << screenY << ")" << endl;


		// draw a red circle
		pointDisplay.draw_circle(screenX, screenY, radius, red, 1);

	}

	CImgDisplay draw_disp(pointDisplay, "Display Points");

	// Shifting this to inside other while loop
	//while (!draw_disp.is_closed() && !draw_disp.is_closed()) {
	//	draw_disp.wait();
	// }

	// Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort(); // Unrecoverable error
	}

	// Get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

	// Create a windowed mode window and its OpenGL context
	g_window = glfwCreateWindow(640, 480, "COMP308 Assignment 2", nullptr, nullptr);
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
	// glfwSetKeyCallback(g_window, keyCallback);
	// glfwSetCharCallback(g_window, charCallback);


	/*
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
	*/


	// Initialize IMGUI




	// Loop until the user closes the window
	while (!glfwWindowShouldClose(g_window)) {

		// Make sure we draw to the WHOLE window
		int width, height;
		glfwGetFramebufferSize(g_window, &width, &height);

		// Main Render
		render(width, height);

		// Render GUI on top
		

		// Swap front and back buffers
			glfwSwapBuffers(g_window);
		glfwPollEvents();
		draw_disp.wait();
	}

	glfwTerminate();
}

