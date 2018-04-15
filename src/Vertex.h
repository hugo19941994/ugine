#pragma once // NOLINT
#include "common.h"

class Vertex {
  public:
	Vertex(float x, float y, float z, float s, float t);
    glm::vec3 position;
	glm::vec2 text_coord;
};
