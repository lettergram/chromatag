cmake_minimum_required(VERSION 2.8)
project( apriltag_mods )
find_package( OpenCV REQUIRED )

add_executable( chromatag chromatag.cpp )
target_link_libraries( chromatag ${OpenCV_LIBS} )
target_link_libraries( chromatag ${CMAKE_SOURCE_DIR}/libapriltag.a )
