#include "render.h"
#include "../render/shading.h"
#include <glad/glad.h>
#include "../render/ui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

double frameStats::frameTime;

bool initGL() {
	renderVAODebug = true;
	logMessage(INFO, "OpenGL init");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logError(CRITICAL, "Failed to initialize GLAD");
		return false;
	}
	glViewport(0, 0, 800, 600);

	// do game rendering here?
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

//wip, need to redesign the render loop, also need to handle ui better.
void doRender() {
	//onFrameStart();
	renderUi();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader testShader = getShader(0);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	glUseProgram(testShader.shaderProgram);
	testShader.setVec4("baseColor", v0Color);
	testShader.setMat4("transform", trans);
	glBindVertexArray(VAO);

	if (renderVAODebug) {
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//onFrameEnd();
	
	

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}