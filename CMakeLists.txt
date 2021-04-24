cmake_minimum_required(VERSION 3.20.1)

project(YANE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})


# Required by olcPixelGameEngine
include_directories(dependencies/olcPixelGameEngine)

find_package(X11 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})


find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

