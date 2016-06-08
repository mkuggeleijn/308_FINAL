#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_geometry.hpp"
	"cgra_math.hpp"
	"opengl.hpp"
	"simple_shader.hpp"
	"simple_image.hpp"
	"geometry.hpp"
	"vEdge.h"
	"VoronoiHandler.h"
	"vVertexPoint.h"
	"CImg.h"
	"vTriangle.h"
	"RiverPoint.h"
	"RiverHandler.h"
	"splineHandler.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"geometry.cpp"
	"spotlight.cpp"
	"vEdge.cpp"
	"VoronoiHandler.cpp"
	"vVertexPoint.cpp"
	"vTriangle.cpp"
	"RiverPoint.cpp"
	"RiverHandler.cpp"
	"splineHandler.cpp"

)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb)