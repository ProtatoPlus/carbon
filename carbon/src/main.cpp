#include "main.h"
#include "d3d12.h"
#include <dxgi1_4.h>
#include "d3d12sdklayers.h"
#include "combaseapi.h"
#include "wlclient.h"
#include <wrl/client.h>
#include "util/logging.h"
#include <iostream>
#include "engine/window.h"
#include "shading.h"
//#pragma comment(lib, "dxgi")
//#pragma comment(lib, "d3d12")

// we will need to rewrite out entrypoint later on, fine for now though.

int main(int argc, char** argv) {
    FreeConsole();
    if (!StartLog()) {
        std::cout << "FAILED TO START LOG" << std::endl;
        return 1;
    }
    logMessage(INFO, "Carbon init started.");

    // we need to handle glfw initilization in a seperate class, possibly a generic "Engine" class to handle basics?
    if (!glfwInit()) {
        std::cerr << "glfw bad no good :(((" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!tryCreateAppWindow()) {
        logError(CRITICAL, "Failed to create app window");
        return 1;
    }

    mainLoop = true;

    /* if (!initd3d()) {
        logError(CRITICAL, "Failed to initialize d3d12");
        return 1;
    }*/
    //  i kinda want to mess around with opengl more before i keep working with d3d12, i still
    //  have a lot to learn about graphics and i need to pace myself and do this the right way.

    //  Might as well learn vulkan for contributions - MRD

    if (!initGL()) {
        return 1;
    }

    if (!initImgui()) {
        return 1;
    }

    if (!loadAndCompile("shaders\\shaders.json")) {
        logError(LOW, "Failed to compile shaders");
    }

    printf("id of retrieved shader: %i", getShader(0).ID);

    while (mainLoop) {

        windowLoop();
        Sleep(1);
    }
    return 42;
}

void closeEngine() {
    mainLoop = false;
}