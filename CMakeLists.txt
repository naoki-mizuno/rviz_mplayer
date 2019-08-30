cmake_minimum_required(VERSION 3.1.3)
project(rviz_mplayer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rviz
  pluginlib
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 ${rviz_QT_VERSION} EXACT REQUIRED
  Core
  Widgets
  Network
)
set(QT_LIBRARIES Qt5::Widgets Qt5::Network)

catkin_package(
  CATKIN_DEPENDS
  roscpp
  rviz
  pluginlib
)

include_directories(
  ${QT_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

# Widget for embedding mplayer
add_library(qmpwidget
  src/qmpwidget.cpp src/qmpwidget.h
)
target_link_libraries(qmpwidget
  ${QT_LIBRARIES}
)

add_library(${PROJECT_NAME}
  src/mplayer_panel.cpp src/mplayer_panel.h
)
target_link_libraries(${PROJECT_NAME}
  qmpwidget
  ${QT_LIBRARIES}
  ${catkin_LIBRARIES}
)
