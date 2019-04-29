cmake_minimum_required(VERSION 3.10)
project(meshPartitioner)

set(CMAKE_CXX_STANDARD 17)

find_package(CGAL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(labelPartitioner)
add_subdirectory(directedPartitioner)
add_subdirectory(ADtreePartitioner)

include_directories(
                    ${CGAL_INCLUDE_DIRS}
                    ${EIGEN3_INCLUDE_DIRS}
                    ${OpenCV_INCLUDE_DIRS}
                    include/
                    labelPartitioner/include
                    ADtreePartitioner/include
                    )

file(GLOB_RECURSE meshPart_SOURCE
                    src/*cpp
                    main.cpp
                    )

add_executable(meshPartitioner ${meshPart_SOURCE})
target_link_libraries(meshPartitioner
                        labelPartitioner
                        directedPartitioner
                        ADtreePartitioner
                        ${CGAL_LIBRARY}
                        ${Boost_LIBRARIES}
                        )

add_executable(labelPartitionerTest test/LabelPartitionerTest.cpp)
target_link_libraries(labelPartitionerTest
                        labelPartitioner
                        ${Boost_LIBRARIES}
                        ${Eigen3_LIBRARY}
                        ${OpenCV_LIBS}
                        )