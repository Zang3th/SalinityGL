############################### Project-Setup ###############################

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter --std=c++17 -g")
project(GameEngine)

############################### Settings ####################################

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/Vendor/GLFW/libglfw.so")
set(IRRKLANG_PATH "${CMAKE_SOURCE_DIR}/Vendor/IrrKlang/libIrrKlang.so")
set(ENGINE_LINKER_FLAGS "-lX11")
set(APPLICATION_LINKER_FLAGS "-ldl -lpthread")

############################### Engine-Core #################################

### Source files ###
file(GLOB_RECURSE SRC_ENGINE_CORE Engine/*.cpp)
file(GLOB_RECURSE SRC_VENDOR Vendor/*.cpp)

### Compile to a static library ###
add_library(GameEngine STATIC ${SRC_ENGINE_CORE} ${SRC_VENDOR})

### Include directories ###
target_include_directories(GameEngine PUBLIC Engine/Core)
target_include_directories(GameEngine PUBLIC Engine/Debug)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Buffers)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Camera)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Renderables)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Renderer)
target_include_directories(GameEngine PUBLIC Engine/Rendering/Resources)

target_include_directories(GameEngine PUBLIC Vendor/Other/include)
target_include_directories(GameEngine PUBLIC Vendor/GLFW/include)
target_include_directories(GameEngine PUBLIC Vendor/IrrKlang/include)
target_include_directories(GameEngine PUBLIC Vendor/Imgui/include)
target_include_directories(GameEngine PUBLIC Vendor/GLM/include)

### Enginge linker flags ###
target_link_libraries(GameEngine PUBLIC ${ENGINE_LINKER_FLAGS})

############################# Demo-Applications #############################

### Packages ###
find_package(OpenGL REQUIRED)

### Source files ###
file(GLOB_RECURSE SRC_GREENWORLD Apps/GreenWorld/src/*.cpp)
file(GLOB_RECURSE SRC_PARTICLESIM Apps/ParticleSim/src/*.cpp)

### Compile to an executable ###
add_executable(GreenWorldDemo Apps/GreenWorld/GreenWorldRun.cpp ${SRC_GREENWORLD})
add_executable(ParticleSimDemo Apps/ParticleSim/ParticleSimRun.cpp ${SRC_PARTICLESIM})

### Include directories ###
target_include_directories(GreenWorldDemo PUBLIC Apps/GreenWorld/src)
target_include_directories(GreenWorldDemo PUBLIC Engine)
target_include_directories(ParticleSimDemo PUBLIC Apps/ParticleSim/src)
target_include_directories(ParticleSimDemo PUBLIC Engine)

### Link against static libraries ###
target_link_libraries(GreenWorldDemo PUBLIC GameEngine)
target_link_libraries(ParticleSimDemo PUBLIC GameEngine)

### Link against dynamic libraries ###
target_link_libraries(GreenWorldDemo PUBLIC ${GLFW_PATH})
target_link_libraries(GreenWorldDemo PUBLIC OpenGL::GL)
target_link_libraries(GreenWorldDemo PUBLIC ${IRRKLANG_PATH})
target_link_libraries(ParticleSimDemo PUBLIC ${GLFW_PATH})
target_link_libraries(ParticleSimDemo PUBLIC OpenGL::GL)
target_link_libraries(ParticleSimDemo PUBLIC ${IRRKLANG_PATH})

### Application linker flags ###
target_link_libraries(GreenWorldDemo PUBLIC ${APPLICATION_LINKER_FLAGS})
target_link_libraries(ParticleSimDemo PUBLIC ${APPLICATION_LINKER_FLAGS})