cmake_minimum_required(VERSION 3.22)
project(argos)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
set (CMAKE_CXX_STANDARD 17)
add_definitions(-DTEK_DEBUG)
add_definitions(-DTEK_PLATFORM_WIN32)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL_FOUND = ${OpenGL_FOUND}")
message(STATUS "OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")

include_directories(
        "extern/opengl/"
)

link_directories(

)

file(GLOB SOURCES
        "src/*.cpp"
        )

add_executable(argos WIN32 ${SOURCES})

target_link_libraries(argos
        ${OPENGL_LIBRARIES}
        )
