#include "DebugHandler.h"

DebugHandler::DebugHandler(InputHandler& _inputHandler, Renderer& _renderer) : inputHandler(_inputHandler), renderer(_renderer)
{
    init();
}

DebugHandler::~DebugHandler()
{
}

///@brief Add the initial text objects and remember their IDs
void DebugHandler::init()
{
    //renderer.addText("", 20, renderer.camera.windowHeight-30, 0.6f);
    upperLeftTextID = renderer.addText("", 20, renderer.windowHeight - 30, 0.6f);

    //renderer.addText("60", renderer.camera.windowWidth-50, renderer.camera.windowHeight-40, 1.0f);
    upperRightTextID = renderer.addText("", renderer.windowWidth - 50, renderer.windowHeight - 40, 1.0);
    renderer.updateTextVBO();
}

///@brief Update the text string by getting the various values from across the program
///Improvements possible: Use a static char array with padding to directly index for the new values instead of using stringstream
///Improve so glSubBuffer works for text vertices
void DebugHandler::updateText()
{
    //Only update text if debug text is enabled or if a text update is required, which will flush the text objects to ""
    if (renderer.displayDebugInfo || renderer.textUpdateRequired) 
    {
    //Time function for debugging
    TimePoint beginTime;
	if (renderer.displayDebugInfo)
		beginTime = std::chrono::high_resolution_clock::now();

    std::ostringstream upperLeftContent;
    std::ostringstream upperRightContent;  

    if (renderer.displayDebugInfo)
    {
        //Upper right content, fps counter
        upperRightContent << int(renderer.fps);
        
        //Upper left counter, coords, camera angles
        upperLeftContent.precision(3); //Change float precision when converting to string
        upperLeftContent << std::fixed;
        upperLeftContent << "x: " << renderer.camera.getPosition().x << ", ";
        upperLeftContent << "y: " << renderer.camera.getPosition().y << ", ";
        upperLeftContent << "z: " << renderer.camera.getPosition().z << "\n";

        upperLeftContent.precision(1);
        upperLeftContent << renderer.camera.getCardinalDirection() << ", ";
        upperLeftContent << "yaw: " << renderer.camera.yaw << ", ";
        upperLeftContent << "pitch: " << renderer.camera.pitch << "\n";
        upperLeftContent.precision(2);
        upperLeftContent << "performance:\n";
        upperLeftContent << "render: " << renderer.renderPerformanceMs << " ms\n";
        upperLeftContent << "input: " << inputHandler.inputPerformanceMs << " ms\n";
        upperLeftContent << "debugtext: " << debugPerformanceMs << " ms\n";
    }

    //Send the updated text content to the renderer
    renderer.updateText(upperLeftTextID, upperLeftContent.str(), 20, renderer.windowHeight - 30);
    renderer.updateText(upperRightTextID, upperRightContent.str(), renderer.windowWidth - 50, renderer.windowHeight - 40);
    renderer.updateTextVBO();

    renderer.textUpdateRequired = false;

    //Time this function
    if (renderer.displayDebugInfo)
		debugPerformanceMs = std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() - beginTime ).count() / 1000.0f;
    }

}