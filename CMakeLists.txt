# Project-Setup
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Release")
project(GameEngine)
set(COMPILER_OPTIONS "-Wall")
set(LINKER_FLAGS "-lglfw3 -lX11 -ldl -pthread")

# Source files
file(GLOB_RECURSE SRC_GREENWORLD apps/greenWorldDemo/src/*.cpp)
file(GLOB_RECURSE SRC_ENGINE engine-core/general/src/*.cpp)
file(GLOB_RECURSE SRC_VENDOR engine-core/vendor/src/*.cpp)

# Static library
add_library(GameEngine STATIC ${SRC_ENGINE} ${SRC_VENDOR})

# Include directories for the library
target_include_directories(GameEngine PUBLIC engine-core/general/include)
target_include_directories(GameEngine PUBLIC engine-core/vendor/include)

# Compiler flags for the library
target_compile_options(GameEngine PUBLIC ${COMPILER_OPTIONS})

# Executables
add_executable(greenWorldDemo apps/greenWorldDemo/greenWorldDemo.cpp ${SRC_GREENWORLD})

# Include directories for the executables
target_include_directories(greenWorldDemo PUBLIC apps/greenWorldDemo/include)

# Compiler flags for the executable
target_compile_options(greenWorldDemo PUBLIC ${COMPILER_OPTIONS})

# Link against static libraries
target_link_libraries(greenWorldDemo PUBLIC GameEngine)

# Linker flags
target_link_libraries(greenWorldDemo PUBLIC ${LINKER_FLAGS})