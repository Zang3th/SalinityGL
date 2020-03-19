# GameEngine with OpenGL

#### Objectives:
            - OpenGL-Wrapper with abstraction
            - Should be easy to set up: 
                 - One line of code for starting/stopping/rendering .. (currently not the case!)
            
#### Already implemented:
            - Little GUI
            - Abstracted VAO, VBO, IB, Display-/Inputmanagement, Shader, Data-/Modelclasses etc.
            - Camerasystem (Firstperson)
            - OBJ-Fileparser
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
            - Animation (created in Blender)
            - AI
            - Proper collsiondetection, maybe via boundingboxes/spheres
            - Enhanced water quality (proper watershader)
            - Improved performance/less rendercalls (maybe Instancing or Batchrenderingsystem)
            
 #### Potential problems and things I try to work on:
            - Too many (unnecessary) dependencies (e.g. OpenCV)
            - Naming conventions: Mostly english (I hope). Sorry though if some comments are in german.
            - Not using separate .h and .cpp files: I'm lazy  
            - And maaaany other things... Overall it's not enterprise level
            
#### Other:
            - Versions before the 25th commit don't have OpenCV included        
