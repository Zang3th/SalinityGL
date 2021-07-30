# Project-Setup
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
project(GameEngine)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic --std=c++17 -g")
set(LINKER_FLAGS "-lglfw3 -lX11 -ldl -lGL -pthread")

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

# Executables
add_executable(greenWorldDemo apps/greenWorldDemo/greenWorldDemo.cpp ${SRC_GREENWORLD})

# Include directories for the executables
target_include_directories(greenWorldDemo PUBLIC apps/greenWorldDemo/include)

# Link against static libraries
target_link_libraries(greenWorldDemo PUBLIC GameEngine)

# Linker flags
target_link_libraries(greenWorldDemo PUBLIC ${LINKER_FLAGS})