cmake_minimum_required(VERSION 3.20.1)

project(YANE)

include_directories(include)
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})



find_package(GLUT REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})


