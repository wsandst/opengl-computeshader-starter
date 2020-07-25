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

int windowWidth = 1024;
int windowHeight = 1024;

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

	renderer.loadVBOs(meshes);

	while (!quit) {
		
		if (inputHandler.windowContext) {
			renderer.render();
			if (frameCount % 180 == 0)
			{
				std::cout << "Frame took " << renderer.deltaTime << " ms (" << 1000 / renderer.deltaTime << " fps)" << std::endl;
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