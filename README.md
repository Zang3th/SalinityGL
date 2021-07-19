# GameEngine in C++ with OpenGL

## I'm currently rewriting the whole engine. Everything before commit 132 was deleted!

### Objectives:
   - Simple OpenGL-Wrapper functions
      - Located in the engine-core
   - Example applications that use the wrapper functions
   - Written in C++17 with OpenGL 4.5

### General functionalities

### Example applications     

### Credits
   - [glad](https://github.com/Dav1dde/glad)
   - [GLFW](https://github.com/glfw/glfw)
   - [Easylogging++](https://github.com/amrayn/easyloggingpp)

### Using & Compiling
   - Currently only for linux, using CMake as a buildsystem
      - Clone the repository
      - Install the dependencies (GLFW) 
         - Take a look at the credits to find the repositories
      - mkdir build
      - cd build/
      - cmake ..
      - make
            
   - Not recommended but there is an old version of the engine (everything before commit 132) that builds on windows
      - Clone the repository and execute the .sln file
            
#### License:
   - This code is provided under a BSD-style license. See the file LICENSE for details.

### Screenshots out of applications that I created with the old engine:

<img src="https://user-images.githubusercontent.com/59279641/77147299-375d0f00-6a8d-11ea-9db8-0d94d04d178b.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147427-7ee39b00-6a8d-11ea-86cd-6ba2ad74efff.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147461-94f15b80-6a8d-11ea-82b8-0a67f637a7c7.PNG"/>  
 
[![Simulation project video](https://user-images.githubusercontent.com/59279641/106173433-34927680-6194-11eb-9cfc-caabc3781d1f.gif)](https://user-images.githubusercontent.com/59279641/106173014-c0f06980-6193-11eb-9ea6-1efb2e17c5a9.mp4)

#### My implementation of the game "Breakout" from "learnopengl.com":
<img src="https://user-images.githubusercontent.com/59279641/91633530-3d1a3180-e9e9-11ea-889c-74dba8e7e700.PNG"/>