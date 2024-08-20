cmake_minimum_required(VERSION 3.5)

project(csv-viewer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(csv-viewer
    src/main.cpp
    src/csvviewer.cpp
    include/csvviewer.h
)

target_include_directories(csv-viewer PRIVATE include)
target_link_libraries(csv-viewer PRIVATE Qt5::Widgets)

# Install target
install(TARGETS csv-viewer DESTINATION bin)