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
	//Render settings
	const char* windowTitle = "OpenGL Compute Shader Starter";
	const bool VSYNCEnabled = true;
	const int MSAALevel = 4; //MSAA Antialiasing: 1 for none, 2 for 4x, 4 for 16x
	//Toggleables from within the engine
	bool displayWireframes = false;
	bool displayText = true;
	bool displayDebugInfo = false;

	//Render variables
	int windowWidth, windowHeight;
	bool isFullscreen = false;
	bool textUpdateRequired = false;

    float deltaTime = 0.0f;
	float fps = 60;
	float renderPerformanceMs = 0;

    Camera camera;

	Renderer(int windowWidth, int windowHeight);

    Renderer();

    ~Renderer();

    void updateDeltatime();

	void init();

    void render();

    void close();

	void drawText();

	void toggleFullscreen();

	void resizeWindow(int width, int height);

	void requestShaderReload();

	void screenshot();

	int addText(std::string text, float x, float y, float scale, glm::vec3 color = {1.0, 1.0, 1.0});

	void updateText(int id, std::string text);

	void updateText(int id, std::string text, float x, float y);

	void loadVBOs(std::vector<Mesh>& meshes);

	void updateTextVBO(bool create=false);


private:
	int minimizedWidth = 1000, minimizedHeight = 1000;
	int screenResHeight, screenResWidth;
	TimePoint lastFrame;
	TimePoint lastFPSframe;

	SDL_Window *window;
    SDL_GLContext glContext;

	//3D shader
    Shader geometryShader;
	std::vector<GeometryVBO> geometryVBOs;
	unsigned int texture;
	unsigned int VBO, VAO;

	//Text related
	Shader textShader;
	std::vector<TextObject> textObjects = std::vector<TextObject>();
	TextVBO textVBO;
	TextHandler textHandler;
	//Freetype library instance, for text rendering
	FT_Library freetype;
	unsigned int textTexture;

    void initOpenGL();

    void initSDL();

	void initFreetype();

	void initGeometry();

	void draw();

	void centerWindow();

	void updateResolution();

};

