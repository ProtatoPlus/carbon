#include "opengl.h"

bool initGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logError(CRITICAL, "Failed to initialize GLAD");
		return false;
	}
	glViewport(0, 0, 800, 600);
}

void doRender() {
	glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);
}