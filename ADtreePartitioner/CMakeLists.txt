cmake_minimum_required(VERSION 3.10)

find_package(CGAL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(
        ${CGAL_INCLUDE_DIRS}
        include/
        ../include/
)

file(GLOB_RECURSE ADtreePartitioner_SOURCE
        src/*cpp
        )

file(GLOB_RECURSE ADtreePartitioner_H
        ./include
        ./../include/types.h
        ./../include/stopwatch.h
        )

add_library(ADtreePartitioner ${ADtreePartitioner_SOURCE} ${ADtreePartitioner_H})

target_link_libraries(ADtreePartitioner
        ${CGAL_LIBRARY}
        ${Boost_LIBRARIES}
        )