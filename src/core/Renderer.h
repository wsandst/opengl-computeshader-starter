#pragma once
#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "glm/glm.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H 

#include "stb_image_write.h"

#include "Shader.h"
#include "Camera.h"
#include "VBOWrapper.h"
#include "Mesh.h"
#include "TextHandler.h"

#include <vector>
#include <algorithm>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point TimePoint;

class Renderer {
public:
	const char* windowTitle = "OpenGL Experimental";
    int windowWidth = 1000, windowHeight = 1000;
	bool isFullscreen = false;
	bool displayText = true;
	bool displayDebugInfo = false;
	bool textUpdateRequired = false;
	//bool movementChange = false;

	const bool VSYNCEnabled = false;
	const int MSAALevel = 2; //MS Antialiasing: 1 for none, 2 for 4x, 4 for 16x

    float deltaTime = 0.0f;
	float fps = 60;
	float renderPerformanceMs = 0;

    Camera camera;

	glm::vec3 textColor;

	Renderer(int windowWidth, int windowHeight);

    Renderer();

    ~Renderer();

    void updateDeltatime();

	void init();

    void render();

    void close();

	void drawText();

	int addText(std::string text, float x, float y, float scale);

	void updateText(int id, std::string text);

	void updateText(int id, std::string text, float x, float y);

	void updateRealtimeTextContent();

	void loadVBOs(std::vector<Mesh>& meshes);

	void toggleFullscreen();

	void resizeWindow(int width, int height);

	void requestShaderReload();

	void screenshot();

	void updateTextVBO(bool create=false);

private:
	int minimizedWidth = 1000, minimizedHeight = 1000;
	int screenResHeight, screenResWidth;

	SDL_Window *window;
    SDL_GLContext glContext;
    float lastFrame = 0.0f;
	float lastFPSframe = 0.0f;

    Shader geometryShader;
	std::vector<GeometryVBO> geometryVBOs;
	unsigned int texture;
	unsigned int VBO, VAO;

	//Text related
	Shader textShader;
	std::vector<TextObject> textObjects = std::vector<TextObject>();
	TextVBO textVBO;
	TextHandler textHandler;

	FT_Library freetype;
	unsigned int textTexture;

	std::clock_t renderTimer;

    void initOpenGL();

    void initSDL();

	void initFreetype();

	void initGeometry();

	void draw();

	void centerWindow();

	void updateResolution();

};

