#pragma once // NOLINT
#include "common.h"

class Vertex {
  public:
	Vertex();
	Vertex(const glm::vec3 position, const glm::vec2 coords, const glm::vec3 normal);
	Vertex(const glm::vec3 position, const glm::vec2 coords, const glm::vec3 normal, const glm::vec3 tan);
    glm::vec3 position;
	glm::vec2 text_coord;
	glm::vec3 normal;
	glm::vec3 tangent;
};
