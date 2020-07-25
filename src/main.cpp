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

	//Screen texture example
	/*renderer.renderType = renderer.ScreenTexture;
	renderer.init();

	std::vector<unsigned char> bitmap(windowWidth * windowHeight * 3);

	for (size_t i = 0; i < bitmap.size(); i += 3) //Filling the bitmap with red
	{
		bitmap[i] = 100;
		bitmap[i + 1] = 0;
		bitmap[i + 2] = 0;
	}

	renderer.updateScreenTexture(&bitmap[0], windowWidth, windowHeight);*/

	//3D Mesh example
	renderer.renderType = renderer.Geometry;
	renderer.init();

	std::vector<float> teapotVertices = FileHandler::loadObj("teapot.obj");
	Mesh teapot = Mesh(Vec3i{ 0, 0, 0 }, teapotVertices);

	std::vector<Mesh> meshes = std::vector<Mesh>();
	meshes.push_back(teapot);

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