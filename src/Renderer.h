#pragma once
#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"
#include "VBOWrapper.h"
#include "Mesh.h"

#include <vector>
#include <unordered_map>

class Renderer {
public:
	const char* windowTitle = "C++ OpenGL Boilerplate";
    int windowWidth = 1024, windowHeight = 1024;
	bool isFullscreen = false;

	const int MSAALevel = 2; //MS Antialiasing: 1 for none, 2 for 4x, 4 for 16x


	enum RenderType {ScreenTexture, Geometry};
	RenderType renderType = ScreenTexture;

    float deltaTime = 0.0f;
    Camera camera;

	Renderer(int windowWidth, int windowHeight);

    Renderer();

    ~Renderer();

    void updateDeltatime();

	void init();

    void render();

    void close();

	void updateScreenTexture(unsigned char* byteArray, int width = 1024, int height = 1024);

	void loadVBOs(std::vector<Mesh>& meshes);

	void toggleFullscreen();

	void resizeWindow(int width, int height);

	void requestShaderReload();

private:
	int minimizedWidth = 1280, minimizedHeight = 720;
	int screenResHeight, screenResWidth;

    Shader screenTextureShader, geometryShader;
    SDL_Window *window;
	std::vector<GeometryVBO> geometryVBOs;

    SDL_GLContext glContext;
    unsigned int VAO, VBO, texture;
    float lastFrame = 0.0f;

    void initOpenGL();

    void initSDL();

	void initGeometry();

	void initScreenTexture();

	//Functions related to drawing 3D objects through rasterizing
	void drawGeometry();

	//Functions related to drawing a screen texture from a byte array
	void drawScreenTexture();

	void centerWindow();

	void updateResolution();
};

