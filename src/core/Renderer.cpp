#include "Renderer.h"

Renderer::Renderer(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	init();
}

Renderer::Renderer()
{
	init();
}

Renderer::~Renderer()
{
}

///@brief Update the current deltatime of the frame, at the end of the frame
void Renderer::updateDeltatime()
{
	TimePoint currentFrame = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentFrame - lastFrame).count() / 1000.0f;

	lastFrame = currentFrame;
	//Calculate average fps every 0.5 seconds
	if (std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - lastFPSframe).count() > 1000)
	{
		fps = 1000 / deltaTime;
		lastFPSframe = currentFrame;
	}
}

///@brief Render the scene
void Renderer::render()
{
	//Measure render time, only if debug mode enabled
	TimePoint beginTime;
	if (displayDebugInfo)
		beginTime = std::chrono::high_resolution_clock::now();

	//Clear screen color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera.calculateProjectionMatrix();
	camera.calculateViewMatrix();

	draw();

	if (displayText)
		drawText();

	SDL_GL_SwapWindow(window);

	//Measure render time, only if debug mode enabled
	if (displayDebugInfo)
		renderPerformanceMs = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() - beginTime ).count() / 1000.0f;
}

///@brief Draw the Geometry VBOs to the screen
void Renderer::draw()
{
	if (displayWireframes)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	geometryShader.use();

	for (auto& it : geometryVBOs) { //Draw chunks
		glm::mat4 matrix = glm::mat4(camera.getProjectionMatrix() * camera.getViewMatrix() * it.translation);

		glBindTexture(GL_TEXTURE_2D, texture);
		geometryShader.setMat4("matrix", matrix);
		glBindVertexArray(it.VAO);
		glDrawArrays(GL_TRIANGLES, 0, it.size);
	}

	if (displayWireframes)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

///@brief Initiate the window, OpenGL and the FreeType font library
void Renderer::init()
{
	camera = Camera(windowWidth, windowHeight);

	initSDL();
	initOpenGL();
	initFreetype();

	initGeometry();

}

///@brief Initiate OpenGL
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

///@brief Initialize SDL2 and set it up for OpenGL rendering
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

	//VSYNC
	SDL_GL_SetSwapInterval(VSYNCEnabled);
}

///@brief Initializes the freetype library and loads a font into a texture atlas with the help of the class TextHandler, later used for text rendering
void Renderer::initFreetype()
{
	if (FT_Init_FreeType(&freetype))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(freetype, "assets/fonts/OpenSans-Regular.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
	else
		std::cout << "Loaded OpenSans font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 24);  
  
	int w = 0;
	unsigned int h = 0;

	//Figure out width and height of the character texture atlas
	for(int i = 0; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			std::cout << "Loading character "<< i << " failed" << std::endl;
			continue;
		}

		w += face->glyph->bitmap.width;
		h = std::max(h, face->glyph->bitmap.rows);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
	std::cout << w << "," << h << std::endl;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textTexture);
	glBindTexture(GL_TEXTURE_2D, textTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	textHandler.textAtlasWidth = w;
	textHandler.textAtlasHeight = h;

	int x = 0;

	float halfPixelAdjustment = 1.0 / (textHandler.textAtlasWidth * 2);

	for(int c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		Character character = {
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x >> 6,
			float(x) / w + halfPixelAdjustment
		};

		textHandler.characters[c] = character;

		x += face->glyph->bitmap.width;
		}
	
	FT_Done_Face(face);
	FT_Done_FreeType(freetype);
}

///@brief Initiate the needed components for 3D geometry rendering
void Renderer::initGeometry()
{
	geometryShader = Shader("shaders/geometry.vert", "shaders/geometry.frag");
	textShader = Shader("shaders/text.vert", "shaders/text.frag");
}

///@brief Hot reload the shaders
void Renderer::requestShaderReload()
{
	geometryShader.reload();
	textShader.reload();
}

///@brief Load the Geometry VBO/VBOs and the text VBO
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

	//Create and update the text VBOS
	updateTextVBO(true);

}

///@brief Toggle SDL2 fullscreen, set the appropriate resolution
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

///@brief Update OpenGL window resolution
void Renderer::updateResolution()
{

	glViewport(0, 0, windowWidth, windowHeight);

	camera.windowWidth = windowWidth;
	camera.windowHeight = windowHeight;

	render();

}

///@brief Resize the SDL2 window and update various components which depend on resolution, ex text positions
void Renderer::resizeWindow(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	updateResolution();
	textUpdateRequired	= true;
}

///@brief Center the window, used when starting the program
void Renderer::centerWindow()
{
	int newScreenPosX = screenResWidth / 2 - windowWidth / 2;
	int newScreenPosY = screenResHeight / 2 - windowHeight / 2;
	SDL_SetWindowPosition(window, newScreenPosX, newScreenPosY);
}

///@brief Add a new text object at the specified positon and scale
int Renderer::addText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	std::vector<TextVertexAttrib> attribs = textHandler.createTextVertices(text, x, y, scale, color);
	TextObject textObject = TextObject(textObjects.size(), x, y, scale, color, text, attribs);
	textObjects.push_back(textObject);
	return textObjects.size() - 1;
}

///@brief Update an existing text object based on index, ie order added. Pos overload
void Renderer::updateText(int id, std::string text)
{
	TextObject& textObject = textObjects[id];
	textObject.vertices = textHandler.createTextVertices(text, textObject.x, textObject.y, textObject.scale, textObject.color);
}

///@brief Same as above. Pos overload
void Renderer::updateText(int id, std::string text, float x, float y)
{
	TextObject& textObject = textObjects[id];
	textObject.x = x;
	textObject.y = y;
	textObject.vertices = textHandler.createTextVertices(text, textObject.x, textObject.y, textObject.scale, textObject.color);
}

///@brief Update the text vertex buffer object with new data
void Renderer::updateTextVBO(bool create)
{
	//Update text vertex buffer data
	std::vector<TextVertexAttrib> vertices; 

	for (size_t i = 0; i < textObjects.size(); i++)
	{
		TextObject& textObject = textObjects[i];
		
		vertices.insert(vertices.end(), textObject.vertices.begin(), textObject.vertices.end());
	}
	if (create)
		textVBO.create(vertices);
	else
		textVBO.update(vertices);

	
}

///@brief Draw text based on the mostly static TextVBO.
void Renderer::drawText()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glDisable(GL_DEPTH_TEST); 
	
	// activate corresponding render state	
	textShader.use();
	glm::mat4 projection = glm::ortho(0.0f, float(camera.windowWidth), 0.0f, float(camera.windowHeight));
	textShader.setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVBO.VAO);

	glBindTexture(GL_TEXTURE_2D, textTexture);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO.VBO);

	glDrawArrays(GL_TRIANGLES, 0, textVBO.size);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

///@brief Prepare for program shutdown, close SDL2
void Renderer::close()
{
	SDL_Quit();
}

///@brief Make a screenshot of the screen and save it to a file in the executable directory, named after the current date and time
void Renderer::screenshot()
{
	//Get timestamp string
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

	std::string date = std::to_string(now->tm_year + 1900)
         + (now->tm_mon < 9 ? "0" : "") + std::to_string(now->tm_mon + 1)
         + (now->tm_mday < 10 ? "0" : "") + std::to_string(now->tm_mday)
		 + "_"
		 + (now->tm_hour < 10 ? "0" : "") + std::to_string(now->tm_hour)
		 + (now->tm_min < 10 ? "0" : "") + std::to_string(now->tm_min)
		 + (now->tm_sec < 10 ? "0" : "") + std::to_string(now->tm_sec);

	///OpenGL screen is flipped vertically
	stbi_flip_vertically_on_write(1);

	//Retrieve a bitmap array of the screen from OpeNGL
	GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    std::vector<char> data(width * height * 3);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

	//Write the bitmap array to file as png
    int saved = stbi_write_png(date.data(), width, height, 3, &data[0], 0);

	std::cout << "Saved screenshot as " << date << ".png\n";

}
