#pragma once
#include "Camera.h"
#include "Renderer.h"
#include "SDL.h"

#include <string>
#include <chrono>

class InputHandler
{
public:
	Camera& camera;
	Renderer& renderer;

	float sensitivity = 0.5;
	bool exit = false;
	bool windowContext = true;

	//For performance measuring
	float inputPerformanceMs = 0;

	void handleInput(float deltaTime);

	InputHandler(Renderer& _renderer, Camera& _camera);
	~InputHandler();
private:
	SDL_Event sdlEvent;
	bool F3Pressed = false;
};

