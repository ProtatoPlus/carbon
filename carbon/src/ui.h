#pragma once
#include <imgui/imgui.h>
#include "engine/window.h"
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

static float v0Color[4];
static float v1Color[4];
static float v2Color[4];

bool initImgui();
void renderUi();