cmake_minimum_required(VERSION 2.8)
project(libjpegcrash)
#set(CMAKE_CXX_FLAGS "--disable-static")
add_executable(crash crash.cpp)

#crash
target_link_libraries(crash QtWebKit OgreMain)

#nocrash
#target_link_libraries(crash QtWebKit)
