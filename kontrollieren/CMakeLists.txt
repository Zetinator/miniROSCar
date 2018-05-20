cmake_minimum_required(VERSION 2.8.3)
project(kontrollieren)

find_package(catkin REQUIRED COMPONENTS
		roscpp 
		std_msgs 
		sensor_msgs
		geometry_msgs
		cmake_modules REQUIRED
		tf
		)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES ${PROJECT_NAME} 
	CATKIN_DEPENDS std_msgs sensor_msgs
)

include_directories(include 
	${catkin_INCLUDE_DIRS}
	)

add_executable(kontrol_trajectory_node src/kontrol_trajectory_node.cpp)

target_link_libraries(kontrol_trajectory_node ${catkin_LIBRARIES})

