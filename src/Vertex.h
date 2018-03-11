#pragma once // NOLINT
#include "common.h"

class Vertex {
  public:
    Vertex(float x, float y, float z);
    glm::vec3 position;
};
