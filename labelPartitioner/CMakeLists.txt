cmake_minimum_required(VERSION 3.10)

find_package(CGAL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
        ${CGAL_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        include/
        ../include/
)

file(GLOB_RECURSE labelPartitioner_SOURCE
        src/*cpp
        )

file(GLOB_RECURSE labelPartitioner_H
        ./include
        ./../include/types.h
        ./../include/stopwatch.h
        )

add_library(labelPartitioner ${labelPartitioner_SOURCE} ${labelPartitioner_H})

target_link_libraries(labelPartitioner
        ${CGAL_LIBRARY}
        ${Boost_LIBRARIES}
        ${Eigen3_LIBRARY}
        ${OpenCV_LIBS}
        )