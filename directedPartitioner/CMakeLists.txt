cmake_minimum_required(VERSION 3.10)

find_package(CGAL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        ${CGAL_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        include/
        ../include/
)

file(GLOB_RECURSE directedPartitioner_SOURCE
        src/*cpp
        )

file(GLOB_RECURSE directedPartitioner_H
        ./include
        ./../include/types.h
        ./../include/stopwatch.h
        )

add_library(directedPartitioner ${directedPartitioner_SOURCE} ${directedPartitioner_H})

target_link_libraries(directedPartitioner
        ${CGAL_LIBRARY}
        ${Boost_LIBRARIES}
        ${OPENGL_LIBRARIES}
        glfw
        )