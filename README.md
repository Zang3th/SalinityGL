# GameEngine in C++ with OpenGL

[![codefactor.io](https://www.codefactor.io/repository/github/zanget3u/gameengine/badge)](https://www.codefactor.io/repository/github/zanget3u/gameengine)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/206a757e7dc642ea9d00d42aad85da56)](https://www.codacy.com/gh/Zanget3u/GameEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Zanget3u/GameEngine&amp;utm_campaign=Badge_Grade)
### Objectives:
   - For my personal educational purposes  
   - Abstraction layer for 2D/3D-Rendering
   - OpenGL-Wrapper-Functions
   - Written in C++17 with OpenGL 4.5
   - Example applications that are using the engine
     - GreenWorld: Nature-Scene (3D)   
     - ParticleSim: Particle/Sandfalling-Simulation (2D)

### Basic functionalities
   - Basic OpenGL-Wrapping (VAO, VBO, IBO, FBO, Errorhandling)
     - Windowcreation
     - Resourcemanagement (Shader, Textures)
     - 2D-Sprite-Rendering
     - 3D-Model-Rendering
     - Camerasystem + Control
     - Cubemaps/Skyboxes
   - File-Management
   - Profiling
   - UI
   - Audio
   - Loading of OBJ-Models
   - Creation of basic meshes

### Additional functionalities
   - Lighting
     - Blinn-Phong
     - Shadow-Rendering
     - Normalmapping
   - Water-Rendering
   - Instanced-Rendering
      - Particles
           - Smoke

### ToDo's   
   - Grass
   - Bloom
   - Lensflare
   - Simple Raytracing
     - Shadows and Lighting in 2D
   - And a lot more ...

### Libraries (Credits to the Creators and Contributors)
   - [glad](https://github.com/Dav1dde/glad)
   - [glfw](https://github.com/glfw/glfw)
   - [easylogging++](https://github.com/amrayn/easyloggingpp)
   - [glm](https://github.com/g-truc/glm)   
   - [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)   
   - [imgui](https://github.com/ocornut/imgui), [imgui::plot_var](https://github.com/ocornut/imgui/wiki/plot_var_example)
   - [irrKlang](https://www.ambiera.com/irrklang/)
   - [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)

### Using & Compiling (currently for Linux with CMake)
   - Tested with Ubuntu 16.04 - 22.04
   - Install the external dependencies if needed

         sudo apt install cmake pkg-config
         sudo apt install libglu1-mesa-dev mesa-common-dev
   - Clone or download the repository and build it
         
         git clone https://github.com/Zanget3u/GameEngine
         mkdir Build
         cd Build
         cmake ..
         make
            
### License:
   - This code is provided under the MIT License. See the file LICENSE for details.

### GreenWorld-Application:
![GreenWorld](Res/Screenshots/GreenWorld/Screenshot_GW_022.png)