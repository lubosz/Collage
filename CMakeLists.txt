cmake_minimum_required(VERSION 2.6)
project(collage)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(
	include
	include/common
	#/usr/include/OIS
	#/usr/include/OGRE
)

set(SOURCES
    ${SRC_DIR}/Input.cpp
    ${SRC_DIR}/System.cpp
    ${SRC_DIR}/RenderEngine.cpp
    ${SRC_DIR}/CollageApplication.cpp
    ${SRC_DIR}/UserInterface.cpp
	${SRC_DIR}/main.cpp
)

add_executable(collage ${SOURCES})

if (APPLE)
	include_directories(
		$ENV{OGRE_HOME}/include
		$ENV{OGRE_HOME}/boost_1_42
	)
	link_directories(
		$ENV{OGRE_HOME}/lib
		$ENV{OGRE_HOME}/lib/release
	)
	# Build 32 cause Ogre 1.7 doesn't support 64bit on OSX
	set(CMAKE_CXX_FLAGS "-m32")
	# Also link all the other libs that Ogre apparently needs, see:
	# http://www.ogre3d.org/forums/viewtopic.php?f=2&t=57997
	set(CMAKE_EXE_LINKER_FLAGS "-m32 -framework Ogre -framework CoreFoundation -framework IOKit -framework ApplicationServices -framework Carbon -framework AppKit")
	target_link_libraries(collage $ENV{OGRE_HOME}/lib/release/libOIS.a)

elseif(UNIX)
	find_package(PkgConfig)
	pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
	message("Found Ogre " ${OGRE_LIBRARY_DIRS}) #/usr/local/lib
	link_directories(${OGRE_LIBRARY_DIRS})
	target_link_libraries(collage OgreMain pthread OIS)

elseif (WIN32)

endif()

