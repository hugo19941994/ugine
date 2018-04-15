#pragma once
#include "common.h"
#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "Shader.h"
#include <vector>

class State {
public:
	static std::shared_ptr<Shader> defaultShader;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;
};
