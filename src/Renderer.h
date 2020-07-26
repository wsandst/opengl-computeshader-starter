#pragma once
#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H 

#include "Shader.h"
#include "Camera.h"
#include "VBOWrapper.h"
#include "Mesh.h"

#include <vector>
#include <unordered_map>

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class Renderer {
public:
	const char* windowTitle = "OpenGL Experimental";
    int windowWidth = 1024, windowHeight = 1024;
	bool isFullscreen = false;

	const int MSAALevel = 2; //MS Antialiasing: 1 for none, 2 for 4x, 4 for 16x

    float deltaTime = 0.0f;


    Camera camera;

	Renderer(int windowWidth, int windowHeight);

    Renderer();

    ~Renderer();

    void updateDeltatime();

	void init();

    void render();

    void close();

	void renderText(std::string text, float x, float y, float scale, glm::vec3 color);

	void updateScreenTexture(unsigned char* byteArray, int width = 1024, int height = 1024);

	void loadVBOs(std::vector<Mesh>& meshes);

	void toggleFullscreen();

	void resizeWindow(int width, int height);

	void requestShaderReload();

private:
	int minimizedWidth = 1280, minimizedHeight = 720;
	int screenResHeight, screenResWidth;

    Shader geometryShader;
	Shader textShader;
    SDL_Window *window;
	std::vector<GeometryVBO> geometryVBOs;

    SDL_GLContext glContext;
    unsigned int VAO, VBO, texture;
	unsigned int textVAO, textVBO;
    float lastFrame = 0.0f;
	float lastFPSframe = 0.0f;
	float fps = 60;

	//Font related
	FT_Library freetype;
	std::map<char, Character> characters;

    void initOpenGL();

    void initSDL();

	void initFreetype();

	void initGeometry();

	void draw();

	void centerWindow();

	void updateResolution();
};

