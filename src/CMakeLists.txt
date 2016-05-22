#
# CGRA Project
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"cgra_geometry.hpp"
	"cgra_math.hpp"
	"opengl.hpp"
	"VoronoiHandler.h"
	"CImg.h"
	"vVertexPoint.h"
	"vArc.h"
	"vEdge.h"
	"vEvent.h"
)


# TODO list your source files (.cpp) here
SET(sources
	"VoronoiHandler.cpp"
	"DotTest.cpp"
	"vVertexPoint.cpp"
	"vArc.cpp"
	"vEvent.cpp"
	"vEdge.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${CGRA_PROJECT} ${headers} ${sources})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE imgui)