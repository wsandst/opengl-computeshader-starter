#pragma once
#include "InputHandler.h"
#include "Renderer.h"

#include <memory>
#include <string>
#include <vector>
#include <sstream>

class DebugHandler
{
    //Tracks variables for the debug menu and updates the text content
private:
    int upperLeftTextID;
    int upperRightTextID;

    //Access to various program values through here
    InputHandler& inputHandler;
    Renderer& renderer;

    float debugPerformanceMs = 0;

public:

    void init();

    void updateText();

    DebugHandler(InputHandler& _inputHandler, Renderer& _renderer);
    ~DebugHandler();
};