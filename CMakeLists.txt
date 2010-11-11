cmake_minimum_required(VERSION 2.6)
project(collage)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(
	include
)

set(SOURCES
	${SRC_DIR}/main.cpp
)

find_package(PkgConfig)
pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
link_directories(${OGRE_LIBRARY_DIRS})

add_executable(collage ${SOURCES})
#target_link_libraries(collage Ogre)
target_link_libraries(collage ${OGRE_LIBRARIES})
