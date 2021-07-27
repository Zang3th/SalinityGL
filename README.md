# GameEngine in C++ with OpenGL

## I'm currently rewriting the whole engine. Everything before commit 67cf366 was deleted!

### Objectives:
   - Simple OpenGL-Wrapper functions
      - Located in the engine-core
   - Example applications that use the wrapper functions
   - Written in C++17 with OpenGL 4.5

### General functionalities
   - Currently none
   
### Example applications  
   - Currently none

### Credits
   - [Easylogging++](https://github.com/amrayn/easyloggingpp)
   - [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
   - [glad](https://github.com/Dav1dde/glad)
   - [GLFW](https://github.com/glfw/glfw)
   - [glm](https://github.com/g-truc/glm)
   - [imgui](https://github.com/ocornut/imgui)
   - [imgui::plot_var](https://github.com/ocornut/imgui/wiki/plot_var_example)

### Using & Compiling
   - Currently only for linux, using CMake as a buildsystem
      - Clone the repository
      - Install the extern dependencies (glm, GLFW) 
         - Take a look at the credits to find the repositories
      - mkdir build
      - cd build/
      - cmake ..
      - make            
   - Not recommended but there is an old version of the engine (everything before commit 67cf366) that builds on windows
      - Clone the repository and execute the .sln file
            
### License:
   - This code is provided under a BSD-style license. See the file LICENSE for details.

### Screenshots out of applications that I created with the old engine:

#### 3D-World with lighting, models and single raycasting to raise/sink tiles
<img src="https://user-images.githubusercontent.com/59279641/77147299-375d0f00-6a8d-11ea-9db8-0d94d04d178b.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147427-7ee39b00-6a8d-11ea-86cd-6ba2ad74efff.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147461-94f15b80-6a8d-11ea-82b8-0a67f637a7c7.PNG"/>  
 
#### 3D-Simulation-Project with bullet3 as a physics engine  
[![Simulation project video](https://user-images.githubusercontent.com/59279641/106173433-34927680-6194-11eb-9cfc-caabc3781d1f.gif)](https://user-images.githubusercontent.com/59279641/106173014-c0f06980-6193-11eb-9ea6-1efb2e17c5a9.mp4)

#### My implementation of the game "Breakout" from "learnopengl.com":
<img src="https://user-images.githubusercontent.com/59279641/91633530-3d1a3180-e9e9-11ea-889c-74dba8e7e700.PNG"/>

### Look of the GreenWorld App:
   ![GreenWorld](res/screenshots/Screenshot_GW_001.png)