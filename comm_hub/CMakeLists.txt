cmake_minimum_required(VERSION 3.0.2)
project(comm_hub)

# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  message_generation
  nav_msgs
  nav_msgs
  roscpp
  serial
  roslib
  rospy
  sensor_msgs
  std_msgs
  tf
)

# add_message_files(
#   DIRECTORY msg
#   FILES
#   param_set.msg
# )

# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

# find_package(OpenCV REQUIRED)

# set(CMAKE_CXX_FLAGS "-std=c++14 ${CMAKE_CXX_FLAGS}")
add_compile_options(-g -std=c++11)

catkin_package(
#   INCLUDE_DIRS
#     include
#   CATKIN_DEPENDS
#     roscpp
#     std_msgs
#     # message_runtime
#   # DEPENDS
#   #   ${LIBS}
)

include_directories(
  include
  # External/Eigen
  # External/Json/include
  ${catkin_INCLUDE_DIRS}
  # ${OpenCV_INCLUDE_DIRS}
)

file(GLOB SOURCE_FILES 
	  src/*.cpp
	)

add_executable(comm_hub_node
  ${SOURCE_FILES}
)
# add_dependencies(comm_hub_node ${${PROJECT_NAME}_EXPORTTED_TARGETS}
#                                    ${catkin_EXPORTED_TARGETS}
#                                    )
# find_package(Python COMPONENTS Development)
# target_include_directories(smart_car_main PRIVATE ${Python_INCLUDE_DIRS} ${catkin_LIBRARIES})
# target_link_libraries(smart_car_main ${Python_LIBRARIES} ${catkin_LIBRARIES})

target_link_libraries(comm_hub_node 
    ${catkin_LIBRARIES}
)


