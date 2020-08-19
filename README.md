# GameEngine with OpenGL

#### Objectives:
            - OpenGL-Wrapper with abstraction
            - Should be easy to set up: 
                 - One line of code for starting/rendering .. (currently not the case!)
            
#### Already implemented:
            - Little GUI
            - Abstracted VAO, VBO, IB, Display-/Inputmanagement, Shader, Data-/Modelclasses etc.
            - Camerasystem (Firstperson)
            - Simple OBJ-Fileparser
            - Terraincreation via Heightmap and texturing via Blendmap
            - Playerclass with movement
            - Audiomanager for sounds and backgroundmusic
            - Skybox
            - Pointlights
            - Normalcalculations per vertex or per triangle
            - Terraineditor:
                 - Verticepicking via raycasting (ray can be visualized)
                 - Picked vertice is colored and can be ascended/descended
                 
#### Plans for future versions:
            - Animation
            - Game AI / pathfinding
            - Proper collsiondetection, maybe via boundingboxes/spheres
            - Enhanced water quality (proper watershader)
            - Particle Rendering System
            - Improved performance and less rendercalls (maybe Instancing or Batch Rendering System)            
            
 #### Potential problems and things I try to work on:
            - Too many (partly unnecessary) dependencies (e.g. OpenCV)
            - Naming conventions: Mostly english (I hope). Sorry though if some comments are in german.
            - Not using separate .h and .cpp files: I'm lazy  
            - And many other things...
            
#### Other:
            - Versions before the 25th commit don't have OpenCV included 
            
#### License:
            - This code is provided under a BSD-style license. See the file LICENSE for details.

#### Screenshots out of a scene that I created with the engine:

 <img src="https://user-images.githubusercontent.com/59279641/77147299-375d0f00-6a8d-11ea-9db8-0d94d04d178b.PNG"/>
 <img src="https://user-images.githubusercontent.com/59279641/77147427-7ee39b00-6a8d-11ea-86cd-6ba2ad74efff.PNG"/>
 <img src="https://user-images.githubusercontent.com/59279641/77147461-94f15b80-6a8d-11ea-82b8-0a67f637a7c7.PNG"/>  
 
 #### Screenshot of my implementation of the game "Breakout" from "learnopengl.com":
 <img src="https://user-images.githubusercontent.com/59279641/90671729-630b3d80-e255-11ea-8410-2512daf577d6.PNG"/>
