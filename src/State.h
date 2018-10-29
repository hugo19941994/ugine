#pragma once
#include "common.h"
#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "Shader.h"
#include "Light.h"
#include <vector>

class State {
public:
	static std::shared_ptr<Shader> defaultShader;
	static std::shared_ptr<Shader> AAShader;
	static std::shared_ptr<Shader> finalShader;
	static std::shared_ptr<Shader> velShader;

	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;

	static glm::mat4 prevModelMatrix;
	static glm::mat4 prevViewMatrix;
	static glm::mat4 prevProjectionMatrix;

	static std::vector<std::shared_ptr<Light>> lights;
	static glm::vec3 ambient;
	static glm::vec3 eyePos;

	static char frame;
	static float jitter;
};
