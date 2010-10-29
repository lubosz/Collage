cmake_minimum_required(VERSION 2.6)
project(collage)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

if (UNIX)
  find_path(OGRE_PLUGIN_DIR RenderSystem_GL.so
    $ENV{OGRE_HOME}/lib
    $ENV{OGRE_HOME}/lib/OGRE
    /usr/bin
    /usr/lib
    /usr/lib/OGRE
    /usr/local/lib/OGRE
    /opt/OGRE/lib/
    /opt/OGRE/plugins
  )
  set(OGRE_PLUGIN_DIR CACHE STRING "Ogre plugin directory")
endif (UNIX)

# find proper ogre version and dependencies
if (UNIX)
  find_package(PkgConfig)
  pkg_check_modules(OGRE REQUIRED OGRE>=1.7 CEGUI OIS)
else (UNIX)
  set(OGRE_INCLUDE_DIRS ${OGRE_SDK_DIR}/include)
  set(OGRE_LIBDIR ${OGRE_SDK_DIR}/lib)
  set(OGRE_LIBRARIES 
    optimized CEGUIBase
	  optimized OgreMain
    optimized OIS
    debug CEGUIBase_d
    debug OgreMain_d
    debug OIS_d
  )
endif (UNIX)

# modify plugins-linux.cfg accordingly to ${OGRE_PLUGIN_DIR}
if (UNIX)
  message(STATUS "Setting Ogre plugin folder in plugins.cfg")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy ${RES_DIR}/plugins-linux.cfg 
    ${CMAKE_BINARY_DIR}/plugins.cfg
  )
  execute_process(
    COMMAND sed -i -e "s_OGRE-PLUGIN-FOLDER_${OGRE_PLUGIN_DIR}_"
    ${CMAKE_BINARY_DIR}/plugins.cfg
  )
  message(STATUS "Setting Ogre plugin folder in plugins.cfg - done")
endif (UNIX)

set(CAMPUS_SOURCES
  ${SRC_DIR}/main.cpp
)

include_directories(${OGRE_INCLUDE_DIRS} ${RES_DIR}/tinyxml)
link_directories(${OGRE_LIBRARY_DIRS})

if (WIN32)
  add_executable(campus WIN32 ${CAMPUS_SOURCES} ${CAMPUS_HEADERS})
else (WIN32)
  add_executable(campus ${CAMPUS_SOURCES} ${CAMPUS_HEADERS})
endif (WIN32)

target_link_libraries(campus ${OGRE_LIBRARIES} tinyxml)

# copy configuration files and dependencies
add_custom_command(
  TARGET campus POST_BUILD
  if (NOT IS_DIRECTORY ${CMAKE_CFG_INTDIR}/Media)
  COMMAND ${CMAKE_COMMAND}
  ARGS -E echo "copying Media"
  COMMAND ${CMAKE_COMMAND}
  ARGS -E make_directory ${CMAKE_CFG_INTDIR}/Media
  COMMAND ${CMAKE_COMMAND}
  
    ARGS -E copy_directory ${RES_DIR}/Media ${CMAKE_CFG_INTDIR}/Media
  endif (NOT IS_DIRECTORY ${CMAKE_CFG_INTDIR}/Media)
)

if (MSVC_IDE)
  add_custom_command(
    TARGET campus POST_BUILD
	COMMAND ${CMAKE_COMMAND}
    ARGS -E echo "copying Ogre dependencies"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_directory ${OGRE_SDK_DIR}/bin/$(ConfigurationName) ${CMAKE_CFG_INTDIR}
  )
else (MSVC_IDE)
  if (WIN32)
    message("NON MSVC_IDE builds under windows currently fully supported. Copying of Ogre dependencies WILL FAIL! (copying of ogre dependencies)")
  endif (WIN32)
endif (MSVC_IDE)

add_custom_command(
  TARGET campus POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E echo "copying media.cfg"
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_if_different ${RES_DIR}/media.cfg ${CMAKE_CFG_INTDIR}/media.cfg
  COMMAND ${CMAKE_COMMAND}
  ARGS -E echo "copying resources.cfg"
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_if_different ${RES_DIR}/resources.cfg ${CMAKE_CFG_INTDIR}/resources.cfg
)

if (UNIX) # WIN32 comes with it's own plugins.cfg from the OGRE_SDK bin directory
  add_custom_command(
    TARGET campus POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E echo "copying plugins.cfg"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different ${CMAKE_BINARY_DIR}/plugins.cfg ${CMAKE_CFG_INTDIR}/plugins.cfg
  )
endif (UNIX)
