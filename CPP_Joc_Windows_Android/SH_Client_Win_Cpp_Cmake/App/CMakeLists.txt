cmake_minimum_required (VERSION 3.8)
set(CMAKE_BUILD_PARALLEL_LEVEL 5)

#This sets PROJECT_NAME to App
project(App
    LANGUAGES C CXX
)

set(CPPFLAGS "${CPPFLAGS} -Wno-extra-semi-stmt")

include("shared/cMakeLists.txt")

# Note. This is needed because the app executable is built only from static libraries and add_executable needs at least 1 source file (empty file is fine).
file(WRITE null.cpp "")
add_executable(${PROJECT_NAME} 
	null.cpp app.rc
)

## Opengl - START
find_package(OpenGL REQUIRED)
## Opengl - END

# Old method to include all src code. Maybe not be the optimal method.
# -Wl,--whole-archive
# src
# src_native
# -Wl,--no-whole-archive
target_link_libraries(${PROJECT_NAME}
	src
	src_native

    ${OPENGL_gl_LIBRARY}
	avrt
	"glfw3.lib"
	"glew32s.lib"

	BulletDynamics
	BulletCollision
	LinearMath
)
