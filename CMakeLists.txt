# Project-Setup
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
project(GameEngine)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic --std=c++17 -g")
set(LINKER_FLAGS "-lX11 -ldl -pthread")

# Packages
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
    message(Error "OpenGL not found")
endif(NOT OpenGL_FOUND)

find_package(glfw3 3.3 REQUIRED)
if(NOT glfw3_FOUND)
    message(Error "glfw3 not found")
endif(NOT glfw3_FOUND)

find_package(glm REQUIRED)
if(NOT glm_FOUND)
    message(Error "glm not found")
endif(NOT glm_FOUND)

# Source files
file(GLOB_RECURSE SRC_GREENWORLD apps/greenWorldDemo/src/*.cpp)
file(GLOB_RECURSE SRC_ENGINE engine-core/general/src/*.cpp)
file(GLOB_RECURSE SRC_VENDOR engine-core/vendor/src/*.cpp)
file(GLOB_RECURSE SRC_IMGUI engine-core/vendor/imgui/*.cpp)

# Static library
add_library(GameEngine STATIC ${SRC_ENGINE} ${SRC_VENDOR} ${SRC_IMGUI})

# Include directories for the library
target_include_directories(GameEngine PUBLIC engine-core/general/include)
target_include_directories(GameEngine PUBLIC engine-core/vendor/include)
target_include_directories(GameEngine PUBLIC engine-core/imgui/include)

# Executable
add_executable(greenWorldDemo apps/greenWorldDemo/greenWorldDemo.cpp ${SRC_GREENWORLD})

# Include directories for the executable
target_include_directories(greenWorldDemo PUBLIC apps/greenWorldDemo/include)

# Link against static libraries
target_link_libraries(greenWorldDemo PUBLIC GameEngine)

# Link against external dependencies/libraries
target_link_libraries(greenWorldDemo PUBLIC OpenGL::GL)
target_link_libraries(greenWorldDemo PUBLIC glfw)
target_link_libraries(greenWorldDemo PUBLIC glm::glm)

# General linker flags
target_link_libraries(greenWorldDemo PUBLIC ${LINKER_FLAGS})