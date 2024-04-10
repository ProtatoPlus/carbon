#pragma once
#include "../util/logging.h"
#include "../engine/window.h"
#include "../../include/glfw/glfw3.h"
static unsigned int vertexShader;
static unsigned int fragmentShader;
static unsigned int shaderProgram;
static unsigned int VBO;
static unsigned int VAO;
bool initGL();
bool compileShaders();
bool createShaderProgram();
void debugGeometry();
void drawTriangle();
void doRender();