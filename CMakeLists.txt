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


if (APPLE)

elseif(UNIX)
	find_package(PkgConfig)
	pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
	link_directories(${OGRE_LIBRARY_DIRS})
	target_link_libraries(collage OgreMain pthread OIS)

elseif (WIN32)
	#set(OGRE_FOLDER /cygdrive/c/Users/lochmann/SDK/Collage)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{OGRE_HOME}\\lib\\release -L$ENV{OGRE_HOME}\\bin\\release")
	include_directories(
		$ENV{OGRE_HOME}/include
		$ENV{OGRE_HOME}/boost_1_42
	)
	
	link_directories(
		$ENV{OGRE_HOME}/boost_1_42/lib
		#$ENV{OGRE_HOME}/lib/release
		$ENV{OGRE_HOME}/bin/release
	)
	#set(LINK_DIRECTORIES 
	#	${LINK_DIRECTORIES}
	#	$ENV{OGRE_HOME}/boost_1_42/lib
	#	$ENV{OGRE_HOME}/lib/release
	#)

	
	add_executable(collage ${SOURCES})

	target_link_libraries(collage 
		#optimized 
		OgreMain
		OIS
		#pthread 
		#$ENV{OGRE_HOME}/bin/release/OIS.dll
		)

endif()

