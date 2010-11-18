cmake_minimum_required(VERSION 2.8)
project(QOgre)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OGRE 1.7 REQUIRED)
set(QT_USE_QTWEBKIT true)
    
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})

#Find headers and cpps
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE INCLUDES src/*.h)

qt4_wrap_cpp (MOC_OUTFILES ${INCLUDES})

add_executable(qOgre ${SOURCES} ${INCLUDES} ${MOC_OUTFILES})

LINK_DIRECTORIES(${OGRE_LIBRARY_DIRS})

#include_directories(
#        /usr/include/OGRE
#        /usr/local/include/OGRE
#	    /usr/include/QtWebkit
#)

target_link_libraries(qOgre ${QT_LIBRARIES} ${OGRE_LIBRARIES})
