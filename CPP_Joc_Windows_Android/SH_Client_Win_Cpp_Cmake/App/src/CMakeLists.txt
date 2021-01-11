project(src
	LANGUAGES C CXX)

file(GLOB_RECURSE HDRS_MAIN FOLLOW_SYMLINKS ./*.h)
set(HDRS ${HDRS_MAIN})

file(GLOB_RECURSE SRCS_MAIN_C FOLLOW_SYMLINKS ./*.c)
file(GLOB_RECURSE SRCS_MAIN_CPP FOLLOW_SYMLINKS ./*.cpp)
set(SRCS ${SRCS_MAIN_C} ${SRCS_MAIN_CPP})

# Note. Also include the headers so they appear as dependencies in te makefile. That has some benefits for the IDE.
add_library(${PROJECT_NAME} STATIC
	${SRCS} ${HDRS}
)
#