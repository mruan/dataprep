cmake_minimum_required (VERSION 2.6)
project (dataprep)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

include_directories(
  /usr/local/include/assimp/
  .)

link_directories(
  /usr/local/lib/
  )

add_executable(v1 src/v1.cpp)
target_link_libraries(v1 assimp)

#add_subdirectory(src)