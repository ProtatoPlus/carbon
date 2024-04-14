#pragma once
#include "../util/logging.h"
#include "../engine/window.h"
#include "../ui.h"
#include "../../include/glfw/glfw3.h"
static unsigned int vertexShader;
static unsigned int fragmentShader;
static unsigned int shaderProgram;
static unsigned int VBO;
static unsigned int VAO;
static bool renderVAODebug;
bool initGL();
bool compileShaders();
bool createShaderProgram();
void debugGeometry();
void drawTriangle();
void doRender();


static int currentFrame();
static double frameStartTime;
static double frameEndTime;
static double deltaTime;
static double frameTime;
void onFrameStart();
void onFrameEnd();

class frameStats {
public:
	static double frameTime;
};

static frameStats currentFrameStats;