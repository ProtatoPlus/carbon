#include "shading.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <nlohmann/json.hpp>
#include "util/logging.h"
#include <fstream>
#include <filesystem>


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

	unsigned int vertex, fragment;
	int success;
	char infoLog[1024];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		logError(LOW, "Error while compiling vertex shader source");
		return;
	}
	logMessage(INFO, "Compiled vertex shader.");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		logError(LOW, "Error while compiling fragment shader source");
		return;
	}
	logMessage(INFO, "Compiled fragment shader");

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		logError(LOW, "Failed to link shader program");
	}
	logMessage(INFO, "Linked shader program");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
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
}

bool loadAndCompile(const char* indexFile) {

	shaderUtil = ShaderUtil();

	logMessage(INFO, "Starting shader compilation...");

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