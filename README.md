# OpenGL Compute Shader Starter Project using SDL2 with C++  
This is a starter OpenGL Compute Shader project which I use as the base for various Compute Shader programs. It is very similar to [my normal opengl-starter](https://github.com/wsandst/opengl-starter), except for the addition of the compute shader features and removal of some of the more advanced 3D features.    
## Features
* Render a texture, generated from a compute shader, on the screen
* Rendering text on the screen, from a loaded font using the FreeType library  
* Easily modifiable main event/rendering loop and input function  
* Freely moving camera with smooth movement. *Note: The camera is implemented, but the standard rendering setup does only render a 2D texture to the screen. If for example a 3D raytracer is desired, some additional code is needed.*  
* Debug menu which lists performance stats, coordinates and more  
* Many useful hotkeys to help development and debugging  
* VSYNC and MSAA Antialiasing can be turned on easily in the Renderer class  

## Dependencies
**SDL2** - for window and input managment  
**GLEW** (static library) - for OpenGL context  
**GLM** - for matrix/vector math  
**FreeType** - for font bitmap creation   
Included in the `external/` folder using git submodules:  
**stb_image, stb_write, tinyobjloader**
  
## Build instructions  
1. Install the required dependencies: [SDL2](https://www.libsdl.org/download-2.0.php), [GLEW](http://glew.sourceforge.net/), [GLM](https://glm.g-truc.net/0.9.9/index.html), [FreeType](https://www.freetype.org/download.html) (`apt-get install libsdl2-dev, libglm-dev, libglew-dev` installs all except FreeType. Visit FreeType website for download)
2. Clone the repo and the submodules (`git submodule update --init --recursive`)
3. Run cmake (`cmake -DCMAKE_BUILD_TYPE=Release ..` in the folder `opengl-starter/build/`)
4. You can now compile/modify the project 

## Controls
**R** - Hot reload shaders  
**C** - Quit the program  
**Escape** - Unfocus window  
**F1** - Toggle text rendering  
**F2** - Take screenshot  
**F3** - Toggle debug menu  
**F11** - Toggle fullscreen  
**WSAD** - Movement  
**Space/Shift** - Move up/down  
**Z/X** - Increase/Decrease max speed  