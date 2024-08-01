#pragma once
#include <glad.h>
#include <glfw3.h>
#include "../render/render.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
enum osEnum {
    UNDEFINED,
    WIN64,
    UNIX
};
static osEnum currentOs;
bool tryCreateAppWindow();
static GLFWwindow* window;
void framebufferSizeCallback(GLFWwindow* pWindow, int iWidth, int iHeight);
void onWindowClose(GLFWwindow* window);
void windowLoop();
// static bool w_dragging;
// static POINT g_LastMousePos;
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// static DWORD threadID = 0;
// static MSG msg;
// DWORD WINAPI AppWindow(LPVOID lpParam);
// void handleSizeChange(HWND hwnd, UINT flag, int width, int height);