cmake_minimum_required(VERSION 2.8)
project( apriltag_mods )
find_package( OpenCV REQUIRED )

add_executable( apriltag apriltag.cpp )
target_link_libraries( apriltag ${OpenCV_LIBS} )
target_link_libraries( apriltag ${CMAKE_SOURCE_DIR}/libapriltag.a )

add_executable( chromatagServer chromatagServer.cpp )
target_link_libraries( chromatagServer ${OpenCV_LIBS} )
target_link_libraries( chromatagServer ${CMAKE_SOURCE_DIR}/libapriltag.a )