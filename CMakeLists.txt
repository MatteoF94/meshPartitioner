cmake_minimum_required(VERSION 3.10)
project(meshPartitioner)

set(CMAKE_CXX_STANDARD 17)

find_package(CGAL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(
                        ${CGAL_INCLUDE_DIRS}
                        include/
                        )

file(GLOB_RECURSE meshPart_SOURCE
                    src/*cpp
                    main.cpp
                    )

add_executable(meshPartitioner ${meshPart_SOURCE})

target_link_libraries(meshPartitioner
                        ${CGAL_LIBRARY}
                        ${Boost_LIBRARIES}
                        )