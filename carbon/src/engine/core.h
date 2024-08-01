#pragma once
#include "Windows.h"
#include <glm/glm.hpp>

class Vec3 {
public:
	Vec3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vec3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	float x;
	float y;
	float z;


	//idfk, this should work
	glm::vec3 ToGlmVec3() {
		glm::vec3 out;
		out.x = x;
		out.y = y;
		out.z = z;
		return out;
	}
};

// basic transform class, no rotation yet
class c_Transform {
public:

	Vec3 position;

	c_Transform(float _x, float _y, float _z) {
		position.x = _x;
		position.y = _y;
		position.z = _z;
	}
};

// stores essential render information (view matrix, proj matrix etc)
class c_Camera {
public:
	c_Transform camera_transform;
	glm::mat4 view;
	glm::mat4 proj;
};

// base object class, all objects in game should inherit this!
class c_Object {
public:
	char* name;

	bool b_IsWorldSpace;
	c_Transform transform;

	glm::mat4 translation;
};

// base level class, each one represents a different scene inside of the game, with specific objects and configurations.
class c_Level {
public:
	c_Object* a_PObjects[1024];
	c_Camera levelCamera;
};


// base world class, stores persistent information across levels (auth tokens, usernames etc)
// need to implement this but this is the design i want.
class c_World {
public:
	c_Level* a_PLevels[1024];
};

// base engine class, will handle windows and essential configuration (buffer sizes, graphics settings etc)
class c_Carbon {
public:
	c_World engineInstanceWorld;
};

void createGlobalEngineInstance(c_Carbon*);