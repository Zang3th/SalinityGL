# GameEngine with OpenGL

#### Objectives:
            - OpenGL-Wrapper functions
                        - Should be easy to use

#### Structure:
            - Core engine functionalities are in "GameEngine-core"
            - "Breakout", "Zanget3uWorld" and "Simulation"
                        - Actual implementations of projects with special functionalities

#### Core engine functionalities (Methods with enough abstraction to be used across multiple projects):
            - Abstracted VAO, VBO, IB, Shader, Texture and Data-classes
            - Camerasystem (Firstperson)
            - Simple OBJ-Fileparser
            - Audiomanager for sounds and backgroundmusic
            - Cubemap texture creation (for skyboxes)
            - Error/Logging-system
            - Filemanagement
            - Simple mesh creation (planes, tiles ...)

#### Project specific functionalities (Working features which are still not abstract enough to be put in the engine core): 
            - Breakout (my implementation of the game from learnopengl.com):
                        - 2D Sprite-Renderer
                        - Game level creation via fileparsing
                        - Text rendering
                        - 2D Particle-Generator
                        
            - Zanget3uWorld:
                        - Abstracted Entity-/Modelclasses
                        - Terraincreation via Heightmap and texturing via Blendmap
                        - Pointlights
                        - Normalcalculations per vertex or per triangle
                        - Terraineditor:
                                    - Verticepicking via raycasting (ray can be visualized)
                                    - Picked vertice is colored and can be ascended/descended
                                    
            - Simulation:
                        - Abstracted Data-/Objectclasses 
                        - Physics engine with bullet3
                        - Instanced Rendering
            
            - Shared across all projects:
                        - Display-/Inputmanagement
                        - Little GUI with ImGUI
                        
#### Plans for future versions:
            - Animation
            - Game AI / pathfinding
            - Enhanced water quality (proper watershader)
            - Batch Rendering System           
            
#### Potential problems and things I try to work on:
            - Naming conventions: Mostly english (I hope). Sorry though if some comments are in german.
            - Not using separate .h and .cpp files: I'm lazy  
            - And many other things...            
            
#### Building:
            - Currently only for Windows/VS19 (Clone the repository and execute the .sln file)
            
#### License:
            - This code is provided under a BSD-style license. See the file LICENSE for details.

#### Screenshots out of a scene that I created with the engine:

<img src="https://user-images.githubusercontent.com/59279641/77147299-375d0f00-6a8d-11ea-9db8-0d94d04d178b.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147427-7ee39b00-6a8d-11ea-86cd-6ba2ad74efff.PNG"/>
<img src="https://user-images.githubusercontent.com/59279641/77147461-94f15b80-6a8d-11ea-82b8-0a67f637a7c7.PNG"/>  
 
#### Screenshot of my implementation of the game "Breakout" from "learnopengl.com":
<img src="https://user-images.githubusercontent.com/59279641/91633530-3d1a3180-e9e9-11ea-889c-74dba8e7e700.PNG"/>

#### Video of the simulation project
<video width="900" height="600" controls>
  <source src="Simulation.mp4" type="video/mp4">
</video>
