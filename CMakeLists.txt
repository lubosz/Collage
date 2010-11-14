cmake_minimum_required(VERSION 2.8)
project(collage)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(
	include
	include/common
	/usr/include/OGRE
)

OPTION(RELEASE "Release" OFF)
OPTION(CHECKSTYLE "Check Style" ON)

IF(UNIX)
	execute_process(COMMAND uname -a OUTPUT_VARIABLE _output OUTPUT_STRIP_TRAILING_WHITESPACE)
	message(${_output})
ENDIF()

#Optimization / Debug
IF(RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
ELSE()
    set(CMAKE_CXX_FLAGS "-O0 -g3") #Debug
ENDIF()

function(copy_plugins_config FROM_PLUGINS_CFG)
	message(STATUS "Copy from ${FROM_PLUGINS_CFG}")
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FROM_PLUGINS_CFG} ${CMAKE_CFG_INTDIR}/bin/plugins.cfg
	)
endfunction(copy_plugins_config)

#Find headers and cpps
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDES include/*.h)

add_executable(collage ${SOURCES} ${INCLUDES})

IF(EXISTS "/usr/bin/python2")
	SET(PYTHON_VER "python2")
ELSE()
	SET(PYTHON_VER "python")
ENDIF()

#check code stlye
if(CHECKSTYLE)
	add_custom_target(CHECK_STYLE ALL
    	COMMAND ${PYTHON_VER} scripts/cpplint.py --filter=-whitespace/tab,-build/header_guard,-build/include ${SOURCES} ${INCLUDES}
		#[DEPENDS depend depend depend ... ]
        #[WORKING_DIRECTORY dir]
        #[COMMENT comment] [VERBATIM]
        #[SOURCES src1 [src2...]]
    )
endif()

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
	copy_plugins_config($ENV{OGRE_HOME}/bin/plugins.cfg)

elseif(UNIX)
	find_package(PkgConfig)
	pkg_check_modules(OGRE REQUIRED OGRE>=1.7 OIS)
	target_link_libraries(collage OgreMain OgreTerrain OgreProperty OIS)
	copy_plugins_config(${CMAKE_CFG_INTDIR}/bin/plugins-unix.cfg)

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
		
	copy_plugins_config($ENV{OGRE_HOME}/bin/plugins.cfg)

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

