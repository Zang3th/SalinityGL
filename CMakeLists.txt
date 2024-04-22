############################### Project-Setup ###############################

cmake_minimum_required(VERSION 3.20)

set(SALINITYGL_ENGINE_VERSION "0.3.0")
set(SALINITYGL_ENGINE_VERSION_SUFFIX "alpha")
add_compile_definitions(SALINITYGL_ENGINE_VERSION="${SALINITYGL_ENGINE_VERSION}")
add_compile_definitions(SALINITYGL_ENGINE_VERSION_SUFFIX="${SALINITYGL_ENGINE_VERSION_SUFFIX}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")

project(SalinityGL
        VERSION     "${SALINITYGL_ENGINE_VERSION}"
        LANGUAGES   CXX
        DESCRIPTION "OpenGL baselayer library in C++"
)

add_compile_options(
        -Waddress
        -Wformat-nonliteral
        -Wformat-security
        -Wformat
        -Winit-self
        -Wmissing-include-dirs
        -Wno-multichar
        -Wno-parentheses
        -Wno-type-limits
        -Wno-unused-parameter
        -Wunreachable-code
        -Wwrite-strings
        -Wpointer-arith
        -Werror
        -Wall
        -Wextra
        -Wpedantic
        -g
)

############################### Settings ####################################

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/Vendor/GLFW/libglfw.so")
set(ENGINE_LINKER_FLAGS "-lX11")
set(APPLICATION_LINKER_FLAGS "-ldl -lpthread")

############################### Engine-Core #################################

### Source files ###
file(GLOB_RECURSE SRC_ENGINE Engine/*.cpp)
file(GLOB_RECURSE SRC_VENDOR Vendor/*.cpp)

### Compile to a static library ###
add_library(SalinityGL STATIC ${SRC_ENGINE} ${SRC_VENDOR})

### Include directories ###
target_include_directories(SalinityGL PUBLIC Engine/Application)
target_include_directories(SalinityGL PUBLIC Engine/Core)
target_include_directories(SalinityGL PUBLIC Engine/Debug)
target_include_directories(SalinityGL PUBLIC Engine/Physics/CellularAutomata)
target_include_directories(SalinityGL PUBLIC Engine/Physics/FluidSimulation)
target_include_directories(SalinityGL PUBLIC Engine/Rendering/Buffers)
target_include_directories(SalinityGL PUBLIC Engine/Rendering/Camera)
target_include_directories(SalinityGL PUBLIC Engine/Rendering/Renderables)
target_include_directories(SalinityGL PUBLIC Engine/Rendering/Renderer)
target_include_directories(SalinityGL PUBLIC Engine/Rendering/Resources)

target_include_directories(SalinityGL PUBLIC Vendor/Glad/include)
target_include_directories(SalinityGL PUBLIC Vendor/GLFW/include)
target_include_directories(SalinityGL PUBLIC Vendor/Imgui/include)
target_include_directories(SalinityGL PUBLIC Vendor/GLM/include)
target_include_directories(SalinityGL PUBLIC Vendor/Miniaudio)
target_include_directories(SalinityGL PUBLIC Vendor/Other/include)

### Engine linker flags ###
target_link_libraries(SalinityGL PUBLIC ${ENGINE_LINKER_FLAGS})

############################# Demo-Applications #############################

### Packages ###
find_package(OpenGL REQUIRED)

### Source files ###
file(GLOB_RECURSE SRC_GREENWORLD Apps/GreenWorld/*.cpp)
file(GLOB_RECURSE SRC_CELLSIM    Apps/CellSim/*.cpp)
file(GLOB_RECURSE SRC_LIQUEFIED  Apps/Liquefied/*.cpp)

### Compile to an executable ###
add_executable(GreenWorld ${SRC_GREENWORLD})
add_executable(CellSim    ${SRC_CELLSIM})
add_executable(Liquefied  ${SRC_LIQUEFIED})

### Include directories ###
target_include_directories(GreenWorld PUBLIC Apps/GreenWorld)
target_include_directories(GreenWorld PUBLIC Engine)
target_include_directories(CellSim    PUBLIC Apps/CellSim)
target_include_directories(CellSim    PUBLIC Engine)
target_include_directories(Liquefied  PUBLIC Apps/Liquefied)
target_include_directories(Liquefied  PUBLIC Engine)

### Link against engine ###
target_link_libraries(GreenWorld PUBLIC SalinityGL)
target_link_libraries(CellSim    PUBLIC SalinityGL)
target_link_libraries(Liquefied  PUBLIC SalinityGL)

### Link against GLFW and OpenGL ###
target_link_libraries(GreenWorld PUBLIC ${GLFW_PATH})
target_link_libraries(GreenWorld PUBLIC OpenGL::GL)
target_link_libraries(CellSim    PUBLIC ${GLFW_PATH})
target_link_libraries(CellSim    PUBLIC OpenGL::GL)
target_link_libraries(Liquefied  PUBLIC ${GLFW_PATH})
target_link_libraries(Liquefied  PUBLIC OpenGL::GL)

### Application linker flags ###
target_link_libraries(GreenWorld PUBLIC ${APPLICATION_LINKER_FLAGS})
target_link_libraries(CellSim    PUBLIC ${APPLICATION_LINKER_FLAGS})
target_link_libraries(Liquefied  PUBLIC ${APPLICATION_LINKER_FLAGS})
