# asd_s
# mE; Manually add this to avoid the large main bullet build CMakeLists;
FILE (STRINGS "../VERSION" BULLET_VERSION)

set(CPPFLAGS "-Wno-all -Wno-error -Wno-extra -Wno-inconsistent-missing-override -Wno-undef")

# Add preprocessor definitions to the compilation of source files. Adds preprocessor definitions to the compiler command line.
#add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS} ${CPPFLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXXFLAGS} ${CPPFLAGS}")
#asd_e


IF(BUILD_BULLET3)
	SUBDIRS(  Bullet3OpenCL Bullet3Serialize/Bullet2FileLoader Bullet3Dynamics Bullet3Collision Bullet3Geometry )
ENDIF(BUILD_BULLET3)


SUBDIRS(   BulletInverseDynamics BulletSoftBody BulletCollision BulletDynamics LinearMath Bullet3Common)


IF(INSTALL_LIBS)
	#INSTALL of other files requires CMake 2.6
	IF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
		IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			# Don't actually need to install any common files, the frameworks include everything
		ELSE (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
			INSTALL(FILES btBulletCollisionCommon.h btBulletDynamicsCommon.h  DESTINATION ${INCLUDE_INSTALL_DIR})
		ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	ENDIF (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.5)
ENDIF(INSTALL_LIBS)
