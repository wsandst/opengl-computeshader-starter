#include "InputHandler.h"

int count = 0;

void InputHandler::handleInput(float deltaTime)
{
	//Measure performance in debug mode
	TimePoint beginTime;
	if (renderer.displayDebugInfo)
		beginTime = std::chrono::high_resolution_clock::now();

	camera.cameraStep = 0.05f * deltaTime;
	float xOffset, yOffset;

	//Check for inputs
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.key.repeat == 0) //Don't listen for repeat events here
		{
		switch (sdlEvent.type) {
		case SDL_KEYDOWN: //Key down event
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_w: //Move forward
				camera.keyForward = true;
				break;
			case SDLK_s: //Move back
				camera.keyBackward = true;
				break;
			case SDLK_a: //Move left
				camera.keyLeft = true;
				break;
			case SDLK_d: //Move right
				camera.keyRight = true;
				break;
			case SDLK_SPACE: //Move up
				camera.keyUp = true;
				break;
			case SDLK_LCTRL: //Move down
				camera.keyDown = true;
				break;

			case SDLK_z: //Change max speed by 2x
				camera.changeMaxVelocity(2);
				std::cout << "Max speed: " << camera.maxVelocity << "\n";
				break;

			case SDLK_x: //Change max speed by 0.5x
				camera.changeMaxVelocity(0.5);
				std::cout << "Max speed: " << camera.maxVelocity << "\n";
				break;

			case SDLK_r: //Hot reload shaders
				std::cout << "Reloading Shaders" << "\n";
				renderer.requestShaderReload();
				break;

			case SDLK_F1: //Toggle text display
				std::cout << "F1: Text display toggled\n";
				renderer.displayText = !renderer.displayText;
				break;

			case SDLK_F2:
				std::cout << "F2: Taking screenshot\n";
				renderer.screenshot();
				break;

			case SDLK_F3: //Toggle coordinate display
			{
				std::cout << "F3: Debug display toggled\n";
				renderer.displayDebugInfo = !renderer.displayDebugInfo;
				renderer.textUpdateRequired	= true;
				break;
			}

			case SDLK_F11: //Toggle fullscreen
				std::cout << "F11: Fullscreen toggled" << "\n";
				renderer.toggleFullscreen();
				break;

			case SDLK_ESCAPE: //Unfocus window
				windowContext = !windowContext;
				if (windowContext) {
					SDL_ShowCursor(0);
					SDL_SetRelativeMouseMode(SDL_TRUE);
					int relativeX, relativeY; //Skip the next relative mouse event, as the mouse has moved around during unfocus
					SDL_GetRelativeMouseState(&relativeX, &relativeY);
				}
				else {
					SDL_ShowCursor(1);
					SDL_SetRelativeMouseMode(SDL_FALSE);
				}
				break;
			case SDLK_c: //Exiting program
				exit = true;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_w:
				camera.keyForward = false;
				break;
			case SDLK_s:
				camera.keyBackward = false;
				break;
			case SDLK_a:
				camera.keyLeft = false;
				break;
			case SDLK_d:
				camera.keyRight = false;
				break;
			case SDLK_SPACE:
				camera.keyUp = false;
				break;
			case SDLK_LCTRL:
				camera.keyDown = false;
				break;
			}
		
		}
		}
		switch (sdlEvent.type) {
		case SDL_MOUSEMOTION:
			if (windowContext) //Mouse only moves camera when the window is in focus, ie the mouse is invisible
			{
				int relativeX, relativeY;
				SDL_GetRelativeMouseState(&relativeX, &relativeY);

				camera.updateView(relativeX * sensitivity, -relativeY * sensitivity);
			}
			break;
		case SDL_WINDOWEVENT:
			switch (sdlEvent.window.event) {
			case SDL_WINDOWEVENT_RESIZED: //Resizing window
				renderer.resizeWindow(sdlEvent.window.data1, sdlEvent.window.data2);
				std::cout << "Window size changed to " << sdlEvent.window.data1 << "x" << sdlEvent.window.data2 << "\n";
				break;

			case SDL_WINDOWEVENT_CLOSE: //Exiting program
				std::cout << "Exiting program" << "\n";
				exit = true;
				break;
			}
			break;
		}
	}

	camera.move();

	if (renderer.displayDebugInfo)
		inputPerformanceMs = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() - beginTime ).count() / 1000.0f;
}

InputHandler::InputHandler(Renderer& _renderer, Camera& _camera) : camera(_camera), renderer(_renderer)
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

InputHandler::~InputHandler()
{
}
