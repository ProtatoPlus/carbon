#include "main.h"
#include "d3d12.h"
#include <dxgi1_4.h>
#include "d3d12sdklayers.h"
#include "combaseapi.h"
#include "wlclient.h"
#include <wrl/client.h>
#include "logging.h"
#include <iostream>
#include "dx.h"
#include "window.h"
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d12")

int main(int argc, char** argv) {
    FreeConsole();
    if (!StartLog()) {
        std::cout << "FAILED TO START LOG" << std::endl;
        return 0;
    }
    logMessage(INFO, "Carbon init started.");
    
    if (!glfwInit()) {
        std::cerr << "glfw bad no good :(((" << std::endl;
    }

    if (!tryCreateAppWindow()) {
        logError(CRITICAL, "Failed to create app window");
        return 1;
    }


    /* if (!initd3d()) {
        logError(CRITICAL, "Failed to initialize d3d12");
        return 1;
    }*/
    //  i kinda want to mess around with opengl more before i keep working with d3d12, i still
    //  have a lot to learn about graphics and i need to pace myself and do this the right way.

    //  Might as well learn vulkan for contributions - MRD


    while (true) {
        glfwPollEvents();
        Sleep(1);
    }
}