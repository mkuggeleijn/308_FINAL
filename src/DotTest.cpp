#include "cgra_math.hpp"
#include "opengl.hpp"
#include "VoronoiHandler.h"
#include "CImg.h"

using namespace std;
using namespace cgra;
using namespace cimg_library;


VoronoiHandler vHandler;
int imageSize = 800;
int density = 100;
int radius = 2;



const unsigned char red[] = { 255,0,0 };
CImg<unsigned char>  pointDisplay(imageSize, imageSize, 1, 3, 0);

int main() {

	vector<vVertexPoint> pointCloud = vHandler.generatePointSet(density);

	//pointCloud = vHandler.generatePointSet(density);
	vHandler.generateVPolys(&pointCloud);

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

	while (!draw_disp.is_closed() && !draw_disp.is_closed()) {
		draw_disp.wait();
	}

}