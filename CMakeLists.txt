cmake_minimum_required(VERSION 2.6)
project(collage)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include_directories(
	include
	/usr/include/OIS
	/usr/include/OGRE
)

set(SOURCES
    ${SRC_DIR}/Input.cpp
    ${SRC_DIR}/RenderEngine.cpp
    ${SRC_DIR}/CollageApplication.cpp
    ${SRC_DIR}/UserInterface.cpp
	${SRC_DIR}/main.cpp
)

find_package(PkgConfig)
pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
link_directories(${OGRE_LIBRARY_DIRS})

add_executable(collage ${SOURCES})
#target_link_libraries(collage Ogre)
target_link_libraries(collage ${OGRE_LIBRARIES})
