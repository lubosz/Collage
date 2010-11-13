cmake_minimum_required(VERSION 2.6)
project(collage)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(
	include
	include/common
)

OPTION(RELEASE "Release" OFF)

#Optimization / Debug
IF(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
ELSE()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
ENDIF()

#Find headers and cpps
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDES include/*.h)

add_executable(collage ${SOURCES} ${INCLUDES})

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
	add_custom_command(
	  TARGET collage POST_BUILD
	  COMMAND ${CMAKE_COMMAND}
	  ARGS -E echo "copying plugins.cfg"
	  COMMAND ${CMAKE_COMMAND}
	  ARGS -E copy_if_different $ENV{OGRE_HOME}/bin/plugins.cfg ${CMAKE_CFG_INTDIR}/bin/plugins.cfg
	)


elseif(UNIX)
	find_package(PkgConfig)
	pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
	target_link_libraries(collage OgreMain OIS)
	
	execute_process(COMMAND uname -a OUTPUT_VARIABLE _output OUTPUT_STRIP_TRAILING_WHITESPACE)
	message(${_output})
	
	#add_custom_command(
	#  TARGET collage POST_BUILD
	#  COMMAND ${CMAKE_COMMAND}
	#  ARGS -E echo "copying plugins.cfg"
	#  COMMAND ${CMAKE_COMMAND}
	#  ARGS -E copy_if_different ${CMAKE_CFG_INTDIR}/config/plugins-unix.cfg ${CMAKE_CFG_INTDIR}/bin/plugins.cfg
	#)

elseif (WIN32)
	#disable some warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-auto-import")
	
	IF(RELEASE)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{OGRE_HOME}/lib/release -L$ENV{OGRE_HOME}/bin/Release")
	ELSE()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{OGRE_HOME}/lib/debug -L$ENV{OGRE_HOME}/bin/Debug")
	ENDIF()
	
	include_directories(
		$ENV{OGRE_HOME}/include
		$ENV{OGRE_HOME}/boost_1_44
	)
	
	link_directories(
		$ENV{OGRE_HOME}/boost_1_42/lib
	)
	
	IF(RELEASE)
		link_directories(
			$ENV{OGRE_HOME}/lib/release
			$ENV{OGRE_HOME}/bin/Release
		)
		target_link_libraries(collage 
			OgreMain
			OIS
		)
	ELSE()
		link_directories(
			$ENV{OGRE_HOME}/lib/debug
			$ENV{OGRE_HOME}/bin/Debug
		)
		target_link_libraries(collage 
			OgreMain_d
			OIS_d
		)
	ENDIF()

	#replace backslashes with slashes	
	STRING(REGEX REPLACE "\\\\" "/" OGREFOLDER "$ENV{OGRE_HOME}")
		message("Ogre Folder: " ${OGREFOLDER})
		
	add_custom_command(
	  TARGET collage POST_BUILD
	  COMMAND ${CMAKE_COMMAND}
	  ARGS -E echo "copying plugins.cfg"
	  COMMAND ${CMAKE_COMMAND}
	  ARGS -E copy_if_different ${OGREFOLDER}/bin/Release/plugins.cfg ${CMAKE_CFG_INTDIR}/bin/plugins.cfg
	)

	SET(OGRE_DDLS
		cg.dll                                Plugin_BSPSceneManager.dll
		libboost_date_time-mgw45-mt-1_44.dll  Plugin_CgProgramManager.dll
		libboost_thread-mgw45-mt-1_44.dll     Plugin_OctreeSceneManager.dll
		libOIS.dll                            Plugin_OctreeZone.dll
		OgreMain.dll                          Plugin_ParticleFX.dll
		OgrePaging.dll                        Plugin_PCZSceneManager.dll
		OgreProperty.dll                      RenderSystem_Direct3D9.dll
		OgreRTShaderSystem.dll                RenderSystem_GL.dll
		OgreTerrain.dll
	)
	foreach(dll ${OGRE_DDLS})
		add_custom_command(
		  TARGET collage POST_BUILD
		  COMMAND ${CMAKE_COMMAND}
		  ARGS -E echo "copying ${dll}"
		  COMMAND ${CMAKE_COMMAND}
		  ARGS -E copy_if_different ${OGREFOLDER}/bin/Release/${dll} ${CMAKE_CFG_INTDIR}/bin/${dll}
		)
	endforeach(dll)
endif()

