#include "shading.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <nlohmann/json.hpp>
#include "../util/logging.h"
#include <fstream>
#include <filesystem>

const char* vertexBackupSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentBackupSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.0, 1.0f, 1.0f);\n"
"}\0";

unsigned int compileBackupShader(fallbackState backupType) {
	unsigned int shaderOut;
	int success;
	switch (backupType) {

	case VERT:
		shaderOut = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shaderOut, 1, &vertexBackupSource, NULL);
		glCompileShader(shaderOut);
		glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &success);
		if (!success) {
			logError(HIGH, "A backup vertex shader failed to compile, something is very wrong.");
			return NULL;
		}
		logMessage(INFO, "Compiled backup vertex shader.");
		return shaderOut;

	case FRAG:
		shaderOut = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shaderOut, 1, &fragmentBackupSource, NULL);
		glCompileShader(shaderOut);
		glGetShaderiv(shaderOut, GL_COMPILE_STATUS, &success);
		if (!success) {
			logError(HIGH, "A backup fragment shader failed to compile, something is very wrong.");
			return NULL;
		}
		logMessage(INFO, "Compiled backup fragment shader.");
		return shaderOut;
	}
}

using json = nlohmann::json;

Shader::Shader(const char* vert, const char* frag, unsigned int shaderID) {

	this->ID = shaderID;

	std::string vertCode;
	std::string fragCode;

	std::ifstream vertFile;
	std::ifstream fragFile;

	vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::filesystem::path vertPath = std::filesystem::current_path() / std::filesystem::path(vert);
		vertFile.open(vertPath);
		fragFile.open(std::filesystem::current_path() / std::filesystem::path(frag));
		std::stringstream vertStream, fragStream;

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		vertCode = vertStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure error)
	{
		logError(LOW, "Failed to open shader source file");
		logError(LOW, strerror(errno));
	}

	const char* vertShaderCode = vertCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	unsigned int vertexBuild, fragmentBuild;

	int success;
	char infoLog[1024];
	bool fallbackFlag0 = false;bool fallbackFlag1 = false;
	fallbackState fallback = NONE;

	vertexBuild = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexBuild, 1, &vertShaderCode, NULL);
	glCompileShader(vertexBuild);
	glGetShaderiv(vertexBuild, GL_COMPILE_STATUS, &success);
	if (!success) {
		logError(LOW, "Error while compiling vertex shader source, using backup");
		fallbackFlag0 = true;
	}
	else {
		logMessage(INFO, "Compiled vertex shader.");
	}

	fragmentBuild = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentBuild, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentBuild);
	glGetShaderiv(fragmentBuild, GL_COMPILE_STATUS, &success);
	if (!success) {
		logError(LOW, "Error while compiling fragment shader source, using backup");
		fallbackFlag1 = true;
	}
	else {
		logMessage(INFO, "Compiled fragment shader");
	}

	shaderProgram = glCreateProgram();
	

	logMessage(INFO, "linking shader program");

	if (fallbackFlag0 || fallbackFlag1) {
		if (fallbackFlag0 && !fallbackFlag1) {
			glAttachShader(shaderProgram, compileBackupShader(VERT));
			glAttachShader(shaderProgram, fragmentBuild);
		}
		if (!fallbackFlag0 && fallbackFlag1) {
			glAttachShader(shaderProgram, vertexBuild);
			glAttachShader(shaderProgram, compileBackupShader(FRAG));
		}
		else {
			glAttachShader(shaderProgram, compileBackupShader(VERT));
			glAttachShader(shaderProgram, compileBackupShader(FRAG));
		}
	}
	else {
		glAttachShader(shaderProgram, vertexBuild);
		glAttachShader(shaderProgram, fragmentBuild);
	}

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		logError(LOW, "Failed to link shader program");
		return;
	}
	logMessage(INFO, "Linked shader program");

	glDeleteShader(vertexBuild);
	glDeleteShader(fragmentBuild);
}

void Shader::useShader() {
	glUseProgram(this->shaderProgram);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->shaderProgram, name.c_str()), value);
}
void Shader::setVec4(const std::string& name, float value[4]) const
{
	glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), value[0], value[1], value[2], value[3]);
	//glUniform4f(glGetUniformLocation(this->shaderProgram, name.c_str()), 1, 1, 1, 1);
}

bool loadAndCompile(const char* indexFile) {

	shaderUtil = ShaderUtil();

	logMessage(INFO, "Starting shader compilation...");

	/*logMessage(INFO, "Starting backup shader compilation...");*/

	/*if (!compileBackupShaders()) {
		logError(HIGH, "One or more backup shaders failed to compile.");
	}*/

	if (indexFile == NULL) {
		logError(LOW, "No shader index file supplied to compiler");
		return false;
	}

	std::ifstream shaderListFile;

	std::ios_base::iostate exceptionMask = shaderListFile.exceptions() | std::ios::failbit;
	shaderListFile.exceptions(exceptionMask);

	std::filesystem::path shaderListFilePath = std::filesystem::current_path()/std::filesystem::path(indexFile);

	try {
		shaderListFile.open(shaderListFilePath);
	}
	catch (std::ios_base::failure& error) {
		logError(LOW, strerror(errno));
		return false;
	}

	json shaderList;

	try {
		shaderList = json::parse(shaderListFile);
	}
	catch (json::parse_error& ex) {
		logError(LOW, "Failed to parse shader index file");
		return false;
	}

	for (auto& shaderInfo : shaderList["shaderList"].items()) {
		char shaderMsg[sizeof(shaderInfo.value()["shaderId"].get<std::string>().c_str()) * 10];
		sprintf_s(shaderMsg, "%s%s", "Compiling shader: ", shaderInfo.value()["shaderId"].get<std::string>().c_str());
		logMessage(INFO, shaderMsg);
		Shader newShader = Shader(shaderInfo.value()["shaderDat"]["vertex"].get<std::string>().c_str(), shaderInfo.value()["shaderDat"]["fragment"].get<std::string>().c_str(), std::abs(atoi(shaderInfo.value()["shaderId"].get<std::string>().c_str())));
		shaderUtil.shaderList.push_back(newShader);
	}
	
	return true;
}

Shader getShader(unsigned int id) {
	for (Shader shader : shaderUtil.shaderList) {
		if (shader.ID == id) {
			return shader;
		}
	}
}