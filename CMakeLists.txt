############################### Project-Setup ###############################

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter --std=c++17 -g")
project(GameEngine)

############################### Settings ####################################

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/vendor/glfw/libglfw.so")
set(IRRKLANG_PATH "${CMAKE_SOURCE_DIR}/vendor/irrKlang/libIrrKlang.so")
set(ENGINE_LINKER_FLAGS "-lX11")
set(APPLICATION_LINKER_FLAGS "-ldl -pthread")

############################### Engine-Core #################################

# Source files
file(GLOB_RECURSE SRC_ENGINE engine-core/src/*.cpp)
file(GLOB_RECURSE SRC_VENDOR vendor/other/src/*.cpp)
file(GLOB_RECURSE SRC_IMGUI vendor/imgui/src/*.cpp)

# Compile to a static library
add_library(GameEngine STATIC ${SRC_ENGINE} ${SRC_VENDOR} ${SRC_IMGUI})

# Include directories
target_include_directories(GameEngine PUBLIC engine-core/include)
target_include_directories(GameEngine PUBLIC vendor/other/include)
target_include_directories(GameEngine PUBLIC vendor/glfw/include)
target_include_directories(GameEngine PUBLIC vendor/imgui/include)
target_include_directories(GameEngine PUBLIC vendor/irrKlang/include)
target_include_directories(GameEngine PUBLIC vendor/glm/glm)

# Linking flags
target_link_libraries(GameEngine PUBLIC ${ENGINE_LINKER_FLAGS})

############################# Demo-Applications #############################

# Packages
find_package(OpenGL REQUIRED)

# Source files
file(GLOB_RECURSE SRC_GREENWORLD apps/greenWorldDemo/src/*.cpp)

# Compile to an executable
add_executable(greenWorldDemo apps/greenWorldDemo/greenWorldDemo.cpp ${SRC_GREENWORLD})

# Include directories
target_include_directories(greenWorldDemo PUBLIC apps/greenWorldDemo/include)

# Link against static libraries
target_link_libraries(greenWorldDemo PUBLIC GameEngine)

# Link against dynamic libraries
target_link_libraries(greenWorldDemo PUBLIC ${GLFW_PATH})
target_link_libraries(greenWorldDemo PUBLIC OpenGL::GL)
target_link_libraries(greenWorldDemo PUBLIC ${IRRKLANG_PATH})

# General linker flags
target_link_libraries(greenWorldDemo PUBLIC ${APPLICATION_LINKER_FLAGS})
