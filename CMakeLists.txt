############################### Project-Setup ###############################

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter --std=c++17 -g")
project(GameEngine)

############################### Settings ####################################

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/Vendor/GLFW/libglfw.so")
set(ENGINE_LINKER_FLAGS "-lX11")
set(APPLICATION_LINKER_FLAGS "-ldl -lpthread")

############################### Engine-Core #################################

### Source files ###
file(GLOB_RECURSE SRC_ENGINE_CORE Engine/*.cpp)
file(GLOB_RECURSE SRC_VENDOR Vendor/*.cpp)

### Compile to a static library ###
add_library(GameEngine STATIC ${SRC_ENGINE_CORE} ${SRC_VENDOR})

### Include directories ###
target_include_directories(GameEngine PUBLIC Engine/Application)
target_include_directories(GameEngine PUBLIC Engine/Core)
target_include_directories(GameEngine PUBLIC Engine/Debug)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Buffers)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Camera)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Renderables)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Renderer)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Resources)

target_include_directories(GameEngine PUBLIC Vendor/Other/include)
target_include_directories(GameEngine PUBLIC Vendor/GLFW/include)
target_include_directories(GameEngine PUBLIC Vendor/Imgui/include)
target_include_directories(GameEngine PUBLIC Vendor/GLM/include)
target_include_directories(GameEngine PUBLIC Vendor/Miniaudio)

### Engine linker flags ###
target_link_libraries(GameEngine PUBLIC ${ENGINE_LINKER_FLAGS})

############################# Demo-Applications #############################

### Packages ###
find_package(OpenGL REQUIRED)

### Source files ###
file(GLOB_RECURSE SRC_GREENWORLD Apps/GreenWorld/*.cpp)
file(GLOB_RECURSE SRC_CELLSIM Apps/CellSim/*.cpp)

### Compile to an executable ###
add_executable(GreenWorld ${SRC_GREENWORLD})
add_executable(CellSim ${SRC_CELLSIM})

### Include directories ###
target_include_directories(GreenWorld PUBLIC Apps/GreenWorld)
target_include_directories(GreenWorld PUBLIC Engine)
target_include_directories(CellSim PUBLIC Apps/CellSim)
target_include_directories(CellSim PUBLIC Engine)

### Link against static libraries ###
target_link_libraries(GreenWorld PUBLIC GameEngine)
target_link_libraries(CellSim PUBLIC GameEngine)

### Link against dynamic libraries ###
target_link_libraries(GreenWorld PUBLIC ${GLFW_PATH})
target_link_libraries(GreenWorld PUBLIC OpenGL::GL)
target_link_libraries(CellSim PUBLIC ${GLFW_PATH})
target_link_libraries(CellSim PUBLIC OpenGL::GL)

### Application linker flags ###
target_link_libraries(GreenWorld PUBLIC ${APPLICATION_LINKER_FLAGS})
target_link_libraries(CellSim PUBLIC ${APPLICATION_LINKER_FLAGS})