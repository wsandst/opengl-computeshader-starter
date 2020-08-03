#include <iostream>
#include <vector>

#define NOMINMAX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image_write.h"

#include "Renderer.h"
#include "InputHandler.h"
#include <iostream>
#include <ctime>

int windowWidth = 1000;
int windowHeight = 1000;

int main(int argc, char** argv) 
{
	bool quit = false;

	Renderer renderer(windowWidth, windowHeight);
	InputHandler inputHandler(renderer, renderer.camera);

	int frameCount = 0;

	renderer.init();

	std::vector<float> vertices = FileHandler::loadObj("diablo.obj");
	Texture texture = FileHandler::loadImage("diablo.png");
	Mesh diablo = Mesh(Vec3i{ 0, 0, 0 }, vertices, texture);

	std::vector<Mesh> meshes = std::vector<Mesh>();
	meshes.push_back(diablo);

	renderer.addText("wsandst demo", 15, 15, 1.0f);

	//FPS counter
	renderer.addText("60", renderer.camera.windowWidth-50, renderer.camera.windowHeight-40, 1.0f);
	//Placeholder for coordinate display toggle
	renderer.addText("", 20, renderer.camera.windowHeight-30, 0.6f);

	renderer.textColor = glm::vec3(1.0f, 1.0f, 1.0f);

	renderer.loadVBOs(meshes);
	//Color could be stored as vertex input. Even scale might work that way.
	//A Text class for every separate text part. This class keeps track of where the vertices are stored. If modified, subBuffer change

	while (!quit) {
		
		if (inputHandler.windowContext) {
			renderer.render();
			if (frameCount % 15 == 0)
			{
				renderer.updateRealtimeTextContent();
			}

			frameCount++;
		}
		if (inputHandler.exit)
		{
			quit = true;
		}
		inputHandler.handleInput(renderer.deltaTime);
		renderer.updateDeltatime();
	}

	renderer.close();
	return 0;
}