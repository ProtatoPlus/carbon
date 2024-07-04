#include "ui.h"
#include "opengl.h"

//gonna overhaul the ui system later

bool initImgui() {
	ImGui::StyleColorsDark();
	
	ImGui_ImplOpenGL3_Init("#version 130");
	return true;
}

void renderUi() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Test ui for carbon");

	ImGui::ColorEdit4("Triangle color", v0Color);
	/*ImGui::ColorEdit4("Vertex 1 color", v1Color);
	ImGui::ColorEdit4("Vertex 2 color", v2Color);*/

	ImGui::End();

	ImGui::Render();
}