#pragma once
#include "../util/logging.h"
#include "../engine/window.h"
#include "../render/ui.h"
#include "../../include/glfw/glfw3.h"
static unsigned int vertexShader;
static unsigned int fragmentShader;
static unsigned int shaderProgram;
static unsigned int VBO;
static unsigned int VAO;
static bool renderVAODebug;
bool initGL();
void doRender();

class frameStats {
public:
	static double frameTime;
};

static frameStats currentFrameStats;