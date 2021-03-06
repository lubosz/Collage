cmake_minimum_required(VERSION 2.8)
project(collage)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

OPTION(COLLAGE_RELEASE "Release" OFF)
OPTION(COLLAGE_FIXBLACKSCREEN "Fix Black screen" ON)
OPTION(COLLAGE_CHECKSTYLE "Check Style" ON)
OPTION(COLLAGE_FREEIMAGE_BUG "Free Image Conflict" OFF)
OPTION(COLLAGE_AUDIO "AUDIO" OFF)

#Optimization / Debug
IF(COLLAGE_RELEASE)
    set(CMAKE_CXX_FLAGS "-O3 -msse4 -mssse3") #Optimized
    set(CMAKE_BUILD_TYPE Release)
ELSE()
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "-O0 -g3 -ggdb") #Debug

ENDIF()

#Warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Woverloaded-virtual -Wuninitialized -Wno-error=unused-parameter")

execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
    message(STATUS "GCC Version >= 4.6")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-attribute=const")
else()
    message(STATUS "GCC Version < 4.6, try to upgrade")      
endif()

function(copy_plugins_config FROM_PLUGINS_CFG)
    message(STATUS "Copy from ${FROM_PLUGINS_CFG}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FROM_PLUGINS_CFG} ${CMAKE_CFG_INTDIR}/bin/plugins.cfg
    )
endfunction(copy_plugins_config)


#replace backslashes with slashes
STRING(REGEX REPLACE "\\\\" "/" OGREFOLDER "$ENV{OGRE_HOME}")

#find ogre cmake macro
if(EXISTS $ENV{OGRE_HOME})
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${OGREFOLDER}/CMake")
else()
  if(EXISTS "/usr/local/lib/OGRE/cmake")
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/local/lib/OGRE/cmake")
  else()
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/lib/OGRE/cmake")
  endif()
endif()
message(STATUS "CMake Module Path: ${CMAKE_MODULE_PATH}")

#set cmake macro flags
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OGRE 1.7 REQUIRED)
FIND_PACKAGE(OIS REQUIRED)
set(QT_USE_QTWEBKIT true)
set(QT_USE_QTTEST true)
set(QT_USE_PHONON true)

#set includes
INCLUDE(${QT_USE_FILE})
include_directories(
    include
    include/common
    ${OGRE_INCLUDE_DIRS}
    ${QT_INCLUDE_DIRS}
)

set(GLOB_DIRS Core Level State Simulation)
file(GLOB_RECURSE INCLUDES include/*.h)

foreach(GLOB_DIR ${GLOB_DIRS})
    file(GLOB_RECURSE GLOB_FILES src/${GLOB_DIR}/*.cpp)
    SET(SOURCES ${SOURCES} ${GLOB_FILES})
endforeach(GLOB_DIR)

#moc qt stuff
set(MOC_CLASSES AppState FrameCapture CollageApplication LevelManager GeneralLevel Level LevelState)
foreach(MOC_CLASS ${MOC_CLASSES})
    SET(MOC_INFILES ${MOC_INFILES} include/${MOC_CLASS}.h)
endforeach(MOC_CLASS)
qt4_wrap_cpp (MOC_OUTFILES ${MOC_INFILES})

#add main.cpp
if(APPLE)
    SET(SOURCES ${SOURCES} "src/main.mm")
else()
    SET(SOURCES ${SOURCES} "src/main.cpp")
endif()

#fix black screen race condition
if(COLLAGE_FIXBLACKSCREEN)
    ADD_DEFINITIONS(-DWITH_FIXBLACKSCREEN)
endif()

#freeimage jpeg collision prevention
if(COLLAGE_FREEIMAGE_BUG)
    ADD_DEFINITIONS(-DFREEIMAGE_BUG)
endif()

if(COLLAGE_AUDIO)
    ADD_DEFINITIONS(-DAUDIO)
endif()

#compile
add_executable(collage ${SOURCES} ${INCLUDES} ${MOC_OUTFILES})

#check code stlye
IF(EXISTS "/usr/bin/python2")
    SET(PYTHON_VER "python2")
ELSE()
    SET(PYTHON_VER "python")
ENDIF()

if(COLLAGE_CHECKSTYLE)
add_custom_target(style ALL
        COMMAND ${PYTHON_VER} scripts/cpplint.py --filter=-whitespace/tab,-build/header_guard,-build/include,-readability/function,-readability/streams ${SOURCES} ${INCLUDES}
)
endif()

add_custom_target(remake
        COMMAND make clean && rm CMakeCache.txt && cmake . -G\"Unix Makefiles\" && make -j`getconf _NPROCESSORS_ONLN`
)


if (APPLE)
    include_directories(
        $ENV{OGRE_HOME}/include
        $ENV{OGRE_HOME}/include/OGRE
        $ENV{OGRE_HOME}/boost_1_42
    )
    link_directories(
        $ENV{OGRE_HOME}/lib
        $ENV{OGRE_HOME}/lib/release
    )
    # Build 32 cause Ogre 1.7 doesn't support 64bit on OSX
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")

    # Also link all the other libs that Ogre apparently needs, see:
    # http://www.ogre3d.org/forums/viewtopic.php?f=2&t=57997
    set(CMAKE_EXE_LINKER_FLAGS "-m32 -framework Ogre -framework CoreFoundation -framework IOKit -framework ApplicationServices -framework Carbon -framework AppKit")
    target_link_libraries(collage $ENV{OGRE_HOME}/lib/release/libOIS.a)
    copy_plugins_config(${CMAKE_CFG_INTDIR}/bin/plugins-apple.cfg)

elseif(UNIX)
    copy_plugins_config(${CMAKE_CFG_INTDIR}/bin/plugins-unix.cfg)
    #free image hack (collision in libjpeg with qt)
    IF(EXISTS /usr/lib/qt/plugins/imageformats/libqjpeg.so)
    	#arch
    	set(FREEIMAGEHACK /usr/lib/qt/plugins/imageformats/libqjpeg.so)
	ELSEIF(EXISTS /usr/lib/qt4/plugins/imageformats/libqjpeg.so)
		#ubuntu
		set(FREEIMAGEHACK /usr/lib/qt4/plugins/imageformats/libqjpeg.so)
	ENDIF()
elseif (WIN32)
    #disable some warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-auto-import")

    include_directories(
        $ENV{OGRE_HOME}/boost_1_44
    )
    copy_plugins_config($ENV{OGRE_HOME}/bin/plugins.cfg)

    #SET(OGRE_DDLS
    #
    #    libboost_date_time-mgw45-mt-1_44.dll
    #    libboost_thread-mgw45-mt-1_44.dll
    #    libOIS.dll
    #    OgreMain.dll
    #    OgrePaging.dll
    #    OgreProperty.dll
    #    RenderSystem_GL.dll
    #
    #)
    #foreach(dll ${OGRE_DDLS})
    #    add_custom_command(
    #      TARGET collage POST_BUILD
    #      COMMAND ${CMAKE_COMMAND}
    #      ARGS -E echo "copying ${dll}"
    #      COMMAND ${CMAKE_COMMAND}
    #      ARGS -E copy_if_different ${OGREFOLDER}/bin/Release/${dll} ${CMAKE_CFG_INTDIR}/bin/${dll}
    #    )
    #endforeach(dll)
endif()

#link
target_link_libraries(collage ${QT_LIBRARIES} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${FREEIMAGEHACK})
