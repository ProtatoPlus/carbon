#include "window.h"
#include "../util/util.h"
#include "../main.h"
#include "../util/buildinf.h"
#include "../util/logging.h"
#include <iostream>
#include <thread>
#include <windows.h>
#define GLFW_INCLUDE_NONE

// Try and create a window, if it fails return NULL
bool tryCreateAppWindow() {
    #ifdef _WIN64
        currentOs = WIN64;
    #endif
    #ifdef UNIX
        currentOs = UNIX;
    #endif

    

    logMessage(INFO, "Attempting to create window");
    window = glfwCreateWindow(800, 600, "Carbon", NULL, NULL);
    if (window == NULL) {
        logError(CRITICAL, "Failed to create window");
    }
    else {
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowCloseCallback(window, onWindowClose);
        glfwMakeContextCurrent(window);
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        return true;
    }
    return NULL;
}

void onWindowClose(GLFWwindow* pWindow) {
    logMessage(INFO, "Closing window");
    closeEngine();
    glfwTerminate();
    glfwDestroyWindow(pWindow);
}

void windowLoop() {
    doRender();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebufferSizeCallback(GLFWwindow* pWindow, int iWidth, int iHeight) {
    glViewport(0, 0, iWidth, iHeight);
}


//fuck this, this actually really sucks.
// DWORD WINAPI AppWindow(LPVOID lpParam) {
//     const wchar_t CLASS_NAME[] = L"CARBON";
//
//     WNDCLASS wc = { };
//
//     wc.lpfnWndProc = WindowProc;
//     wc.hInstance = GetModuleHandle(NULL);
//     wc.lpszClassName = CLASS_NAME;
//     RegisterClass(&wc);
//     logMessage(INFO, "Creating window");
//     cl_hwnd = CreateWindowEx(
//         0,
//         L"CARBON",
//         appName,
//         WS_OVERLAPPEDWINDOW,
//         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
//         NULL,
//         NULL,
//         GetModuleHandle(NULL),
//         NULL
//     );
//     if (cl_hwnd == NULL) {
//         logError(CRITICAL, "Failed to create window");
//         return NULL;
//     }
//
//     threadID = GetCurrentThreadId();
//
//     // create thread message queue
//     PeekMessage(&msg, 0, WM_USER, WM_USER, PM_NOREMOVE);
//
//     while (GetMessage(&msg, NULL, 0, 0))
//     {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }
//     return NULL;
// }
//
// // window procedure logic, no experience with this so its gonna be ass (ill rewrite it later)
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//     switch (uMsg) {
//     case WM_LBUTTONDOWN: // Left mouse button down
//         w_dragging = true;
//         GetCursorPos(&g_LastMousePos);
//         ScreenToClient(hwnd, &g_LastMousePos);
//         SetCapture(hwnd); // Capture mouse input
//         break;
//
//     case WM_LBUTTONUP: // Left mouse button up
//         w_dragging = false;
//         ReleaseCapture(); // Release mouse capture
//         break;
//
//     case WM_MOUSEMOVE: // Mouse movement
//         if (w_dragging) {
//             POINT currentPos;
//             GetCursorPos(&currentPos);
//             int deltaX = currentPos.x - g_LastMousePos.x;
//             int deltaY = currentPos.y - g_LastMousePos.y;
//             // SetWindowPos(hwnd, NULL,
//             //     (int)(currentPos.x - g_LastMousePos.x),
//             //     (int)(currentPos.y - g_LastMousePos.y),
//             //     0, 0, SWP_NOSIZE | SWP_NOZORDER); // Move the window
//             // g_LastMousePos = currentPos;
//             // shit sucks
//         }
//         break;
//
//     case WM_DESTROY:
//
//         return NULL; // Quit the application
//         break;
//
//
//     case WM_SIZE: {
//         int width = LOWORD(lParam);
//         int height = HIWORD(lParam);
//         handleSizeChange(hwnd, (UINT)wParam, width, height);
//     }
//         break;
//     default:
//         return DefWindowProc(hwnd, uMsg, wParam, lParam);
//         break;
//     }
// }
//
// //idfk
// void handleSizeChange(HWND hwnd, UINT flag, int width, int height) {
// 	RECT rect;
// 	GetClientRect(hwnd, &rect);
// 	RECT desiredRect = { 0, 0, width, height };
// 	AdjustWindowRect(&desiredRect, GetWindowLong(hwnd, GWL_STYLE), FALSE);
// 	int newWidth = desiredRect.right - desiredRect.left;
// 	int newHeight = desiredRect.bottom - desiredRect.top;
// 	SetWindowPos(hwnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);
// }
// Code seems to be about handleling user changing window size btw - Mrdab