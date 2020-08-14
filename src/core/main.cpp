#include <iostream>
#include <vector>

#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image_write.h"

#include <iostream>
#include <ctime>

#include "Renderer.h"
#include "InputHandler.h"
#include "DebugHandler.h"

int windowWidth = 1000;
int windowHeight = 1000;

int main(int argc, char** argv) 
{
	bool quit = false;

	//Main class instances
	Renderer renderer(windowWidth, windowHeight);
	InputHandler inputHandler(renderer, renderer.camera);
	DebugHandler debugHandler(inputHandler, renderer);

	//Load a model and a texture
	std::vector<float> vertices = FileHandler::loadObj("diablo.obj");
	Texture texture = FileHandler::loadImage("diablo.png");
	Mesh diablo = Mesh(glm::vec3{ 0, 0, 0 }, vertices, texture);

	std::vector<Mesh> meshes = std::vector<Mesh>();
	meshes.push_back(diablo);

	//Add some static text
	int demoTextID = renderer.addText("wsandst demo", 15, 15, 1.0f);
	renderer.textColor = glm::vec3(1.0f, 1.0f, 1.0f);

	renderer.loadVBOs(meshes);

	int frameCount = 0;

	//Main event loop
	while (!quit) {
		
		if (inputHandler.windowContext) { //Window in focus, render frames
			if (frameCount % 15 == 0 || renderer.textUpdateRequired) //Update debug text every 15 frames, or if a text update is requested by the renderer
			{
				debugHandler.updateText();
			}

			//Render
			renderer.render();

			frameCount++;
		}
		if (inputHandler.exit)
		{
			quit = true;
		}
		//Input handling
		inputHandler.handleInput(renderer.deltaTime);
		renderer.updateDeltatime();
	}

	//Exit program
	renderer.close();
	return 0;
}