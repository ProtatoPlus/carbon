#ifndef SHADER_H
#define SHADER_H
#endif

#include <list>
#include <string>

bool loadAndCompile(const char[]);

inline unsigned int backupVertexShader;
inline unsigned int backupFragmentShader;

enum fallbackState {
	VERT,
	FRAG,
	COMPUTE,
	NONE
};

class Shader {
public:

	unsigned int ID;

	unsigned int shaderProgram;

	Shader(const char* vert, const char* frag, unsigned int shaderID);

	void useShader();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec4(const std::string& name, float value[4]) const;

	bool fallbackVert;
	bool fallbackFrag;
};

class ShaderUtil {
public:
	std::list<Shader> shaderList;
};

static ShaderUtil shaderUtil;





Shader getShader(unsigned int id);