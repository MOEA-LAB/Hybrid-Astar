cmake_minimum_required(VERSION 3.7)
project(pure_pursuit)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
  serial_ros
)

catkin_package(
  INCLUDE_DIRS
  include
  CATKIN_DEPENDS
  roscpp
  message_runtime
  DEPENDS
  ${LIBS}
)

include_directories(
  include
  External/Eigen
  ${catkin_INCLUDE_DIRS}
)
include_directories()

file(GLOB SOURCE_FILES
  src/*.cpp
)

add_executable(pure_pursuit
  ${SOURCE_FILES}
)

add_dependencies(pure_pursuit ${${PROJECT_NAME}_EXPORTTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(pure_pursuit ${catkin_LIBRARIES} yaml-cpp)
