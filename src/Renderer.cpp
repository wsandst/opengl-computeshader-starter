#include "Renderer.h"
#define GLEW_STATIC
#include "GL/glew.h"

Renderer::Renderer(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::updateDeltatime()
{
	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.calculateProjectionMatrix();
	camera.calculateViewMatrix();

	draw();

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	SDL_GL_SwapWindow(window);
}

void Renderer::draw()
{
	geometryShader.use();

	for (auto& it : geometryVBOs) { //Draw chunks
		glm::mat4 matrix = glm::mat4(camera.getProjectionMatrix() * camera.getViewMatrix() * it.translation);

		glBindTexture(GL_TEXTURE_2D, texture);
		geometryShader.setMat4("matrix", matrix);
		glBindVertexArray(it.VAO);
		glDrawArrays(GL_TRIANGLES, 0, it.size);
	}
}

void Renderer::init()
{
	camera = Camera(windowWidth, windowHeight);

	initSDL();
	initOpenGL();

	initGeometry();

}

void Renderer::initOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Wireframes
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO), 0) {
		std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
	}

	//Get monitor resolution for fullscreen toggling
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	screenResHeight = DM.h;
	screenResWidth = DM.w;

	//Antialiasing
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAALevel*MSAALevel);
	glEnable(GL_MULTISAMPLE);

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	glContext = SDL_GL_CreateContext(window);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetSwapInterval(1);
}

void Renderer::initGeometry()
{
	geometryShader = Shader("shaders/geometry.vert", "shaders/geometry.frag");
}

void Renderer::requestShaderReload()
{
	geometryShader.reload();
}

void Renderer::loadVBOs(std::vector<Mesh>& meshes)
{
	/*for (auto &mesh : meshes)
	{
		geometryVBOs.push_back(GeometryVBO(mesh.pos, mesh.vertices));
	}*/

	geometryVBOs.push_back(GeometryVBO(meshes[0].pos, meshes[0].vertices));

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, meshes[0].texture.width, meshes[0].texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, meshes[0].texture.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(meshes[0].texture.data);
}

void Renderer::toggleFullscreen() {

	isFullscreen = !isFullscreen;

	if (isFullscreen)
	{
		minimizedWidth = windowWidth;
		minimizedHeight = windowHeight;
		SDL_SetWindowSize(window, screenResWidth, screenResHeight);
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		windowWidth = screenResWidth;
		windowHeight = screenResHeight;

	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, minimizedWidth, minimizedHeight);
		windowWidth = minimizedWidth;
		windowHeight = minimizedHeight;
	}

	updateResolution();
}

void Renderer::updateResolution()
{

	glViewport(0, 0, windowWidth, windowHeight);

	camera.windowWidth = windowWidth;
	camera.windowHeight = windowHeight;

	render();

}

void Renderer::resizeWindow(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	updateResolution();
}

void Renderer::centerWindow()
{
	int newScreenPosX = screenResWidth / 2 - windowWidth / 2;
	int newScreenPosY = screenResHeight / 2 - windowHeight / 2;
	SDL_SetWindowPosition(window, newScreenPosX, newScreenPosY);
}

//Update the covering screen texture with a new texture
void Renderer::updateScreenTexture(unsigned char* bytearray, int width, int height)
{
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytearray);

	int newWidth = (width / (float)height) * windowHeight;
	resizeWindow(newWidth, windowHeight);
	centerWindow();

}

void Renderer::close()
{
	SDL_Quit();
}
