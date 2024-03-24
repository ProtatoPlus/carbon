#pragma once
#include <Windows.h>
#include <glfw3.h>
enum osEnum {
    UNDEFINED,
    WIN64,
    UNIX
};
static osEnum currentOs;
bool tryCreateAppWindow();
static GLFWwindow* window;
void onWindowClose(GLFWwindow* window);
// static bool w_dragging;
// static POINT g_LastMousePos;
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// static DWORD threadID = 0;
// static MSG msg;
// DWORD WINAPI AppWindow(LPVOID lpParam);
// void handleSizeChange(HWND hwnd, UINT flag, int width, int height);