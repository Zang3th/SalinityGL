# SalinityGL

[![Build](https://github.com/Zanget3u/GameEngine/actions/workflows/build.yml/badge.svg)](https://github.com/Zanget3u/GameEngine/actions/workflows/build.yml)
[![Static analysis](https://github.com/Zang3th/GameEngine/actions/workflows/static_analysis.yml/badge.svg)](https://github.com/Zang3th/GameEngine/actions/workflows/static_analysis.yml)
![OS](https://img.shields.io/badge/OS-Linux-blue)
![Release](https://img.shields.io/badge/Release-v0.1.1-blue)

## Introduction

SalinityGL is an OpenGL baselayer library for creating (3D) visualization applications. The goal is to create a library that is easy to use and lightweight. SalinityGL provides a thin OpenGL abstraction layer and some basic helper functions to support the process of building rendering applications.

SalinityGL is written in C++17 with OpenGL 4.5. Currently, only Linux is supported. To get started, take a look at the [building and compiling](https://github.com/Zang3th/GameEngine/tree/master#building-and-compiling) section.

During development, several [demo applications](https://github.com/Zang3th/GameEngine/tree/master#demo-applications) were created to test the engine's capabilities. These apps are built and compiled separately from the engine core, but a global parameter file is maintained for the configuration of some engine-to-app settings.

The development of this library also serves my educational purposes.

SalinityGL is licensed under the [MIT LICENSE](https://github.com/Zang3th/GameEngine/blob/master/LICENSE).

## Project structure

    SalinityGL/
    ├── Apps/                       # Demo applications
    │   ├── GreenWorld/             # 3D nature scene
    │   └── CellSim/                # Cellular automata with 3D cubes (in development) ⌛
    ├── Engine/
    │   ├── Application/            # Application and interface stuff
    │   ├── Core/                   # Utilities and core engine functionalities
    │   ├── Debug/                  # Logging and error handling
    │   ├── Rendering/              # GL stuff, buffers, renderer ...
    │   └── Engine.hpp              # Main header for include in the applications
    ├── Res/                        # Assets, sounds and screenshots
    └── Vendor/                     # Third party libraries

## Base layer functionalities

- Thin OpenGL-Wrapping
  - VAO, VBO, IBO, FBO
  - Resource management (shaders, textures)
  - Error handling
- Window creation
  - UI
- Camera + input management
- Rendering
  - 2D-Sprite-Rendering
  - 3D-Model-Rendering
    - Loading of OBJ-Models
    - Dynamic mesh creation (planes, terrains)
    - Cubemaps/Skyboxes
  - Lighting
    - Blinn-Phong
    - Shadow rendering
    - Normal mapping
  - Water rendering
  - Instanced rendering
    - Flexible 2D particle system
      - Smoke
    - Cellular automata system (in development) ⌛
- File management
- Profiling
- Audio

## Demo applications

### GreenWorld

Nature scene with water rendering, normal mapped objects, and a particle system to display smoke.

![GreenWorld_far](Res/Screenshots/GreenWorld/Screenshot_GW_024_far.png)
![GreenWorld_close](Res/Screenshots/GreenWorld/Screenshot_GW_024_near.png)

### CellSim (in development) ⌛

Cellular automata system with 3D cells/cubes. The plan is to simulate and render >1M cells/cubes at a good framerate. The system should include physics (multithreaded) and pretty graphics (lighting, shadows).

![CellSim](Res/Screenshots/CellSim/Screenshot_CS_008.png)

## Building and compiling

The engine and the demo applications are compiled separately. The engine is compiled into a static library and then included in the demo applications' binary. All external libraries ([Vendor/](https://github.com/Zang3th/GameEngine/tree/master/Vendor)) are getting directly included (via source) into the engine and need no separate install. The only exception is GLFW, which gets linked dynamically (.so file is provided).

### What you need

- Linux
  - Tested with Ubuntu 16.04 - 22.04
- C++17 compiler
  - Tested Clang and GCC
- CMake 3.16 or newer
- OpenGL 4.5 compatible graphics card and driver

### How to get started

- Install the external dependencies (if needed)

      apt install cmake pkg-config
      apt install libglu1-mesa-dev mesa-common-dev

- Clone/download the repository or a particular release and build it

      git clone https://github.com/Zang3th/SalinityGL
      mkdir Build
      cd Build
      cmake ..
      make -j

- Run the demo applications

## Releases

| **Version**                                                        | **Date**   | **Commit [Count / ID]** | **Lines of code [Apps / Engine]** | **Notes**                               |
|--------------------------------------------------------------------|------------|-------------------------|-----------------------------------|-----------------------------------------|
| [0.1.1](https://github.com/Zang3th/GameEngine/releases/tag/v0.1.1) | 14.01.2023 | [255 / d425a33]         | [480 / 4250]                      | Completion of the *GreenWorld* demo app |
| [0.1.0](https://github.com/Zang3th/GameEngine/releases/tag/v0.1.0) | 11.06.2022 | [229 / 218a55e]         | [575 / 4110]                      | First official release                  |

- 07/2021 - 09/2021: Rewrite of the engine core
- 01/2020 - 09/2020: Some very early projects (still under Windows)

## Roadmap

I always worked *on and off* on this project, but I try to make more regular releases until the end of 2024.

### Release preview

- v0.2.0: Completion of the base foundation of the *CellSim* app
- v0.2.1: Integration of more cell types
- v0.2.2: Improvements to the engine core
- v0.2.3: Performance improvements and multithreading

### Backlog

- Grass simulation
- Some raytracing project

## External libraries

**Thanks to all the creators and contributors of these projects!**

| **Library**                                                          | **Version** | **Commit** | **Updated in Engine** | **Functionality**           |
|--------------------------------------------------------------------- |-------------|------------|-----------------------|-----------------------------|
| [GLFW](https://github.com/glfw/glfw)                                 | 3.3.8       | 7482de6    | 10.10.2023            | Window and input management |
| [glad](https://github.com/Dav1dde/glad)                              | 2.0.4       | d08b1aa    | 10.10.2023            | OpenGL function loading     |
| [imgui](https://github.com/ocornut/imgui)                            | 1.89.9      | c6e0284    | 20.09.2023            | GUI                         |
| [miniaudio](https://github.com/mackron/miniaudio)                    | 0.11.18     | 3898fff    | 20.09.2023            | Audio                       |
| [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) | 2.28        | 3ecc60f    | 20.09.2023            | Image loading               |
| [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)      | /           | 853f059    | 20.09.2023            | Wavefront obj loading       |
| [GLM](https://github.com/g-truc/glm)                                 | 0.9.9.8     | bf71a83    | 23.07.2021            | Mathematics                 |
| [Easylogging++](https://github.com/amrayn/easyloggingpp)             | 9.96.7      | 5181b40    | 19.07.2021            | Logging                     |

## Credits

This project uses [PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source), a static analyzer for C, C++, C#, and Java code that's free to use for open source projects and students.
