#include "opengl.h"
#include "../render/shading.h"
#include <glad/glad.h>
#include "../render/ui.h"

double frameStats::frameTime;

bool initGL() {
	renderVAODebug = true;
	logMessage(INFO, "OpenGL init");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logError(CRITICAL, "Failed to initialize GLAD");
		return false;
	}
	glViewport(0, 0, 800, 600);

	debugGeometry();

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void debugGeometry() {
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


//wip, need to redesign the render loop, also need to handle ui better.
void doRender() {
	//onFrameStart();
	renderUi();

	glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader testShader = getShader(0);

	glUseProgram(testShader.shaderProgram);
	testShader.setVec4("baseColor", v0Color);
	glBindVertexArray(VAO);

	if (renderVAODebug) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//onFrameEnd();
	
	

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void onFrameStart() {
	frameStartTime = glfwGetTime();
}

void onFrameEnd() {
	frameEndTime = glfwGetTime();
	double newFrameTime = frameEndTime - frameStartTime;
	currentFrameStats.frameTime += newFrameTime;
}