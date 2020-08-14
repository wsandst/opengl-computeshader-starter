# OpenGL Starter Project using SDL2 with C++  
This is a starter OpenGL project which I use as the base for various hobby OpenGL projects. Several classes simplify the interface to OpenGL and SDL2,
and many common features are implemented already which helps you from having to redevelop the same features many times. This project was built ontop of my [previous OpenGL boilerplate project](https://github.com/wsandst/opengl-boilerplate).
## Features
* Rendering models with textures and simple lighting  
* Rendering text on the screen, from a loaded font using the FreeType library  
* Easily modifiable main event/rendering loop and input function  
* Freely moving camera with smooth movement  
* Debug menu which lists performance stats, coordinates and more  
* Many useful hotkeys to help development and debugging  
* VSYNC and MSAA Antialiasing can be turned on easily in the Renderer class  

![Example Image](https://i.postimg.cc/vB6rVJrY/20200814-220236.png)

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
**WSAD** - Movement  
**Space/Shift** - Move up/down  
**Escape** - Unfocus window  
**Z/X** - Increase/Decrease max speed  
**R** - Hot reload shaders  
**C** - Quit the program  
**F1** - Toggle text rendering  
**F2** - Take screenshot  
**F3** - Toggle debug menu  
**F4** - Toggle wireframes  
**F11** - Toggle fullscreen  