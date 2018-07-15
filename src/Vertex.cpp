#include "Vertex.h"
Vertex::Vertex() {};
Vertex::Vertex(const glm::vec3 position, const glm::vec2 coords, const glm::vec3 normal) : position(position), text_coord(coords), normal(normal) {};
Vertex::Vertex(const glm::vec3 position, const glm::vec2 coords, const glm::vec3 normal, const glm::vec3 tan) : position(position), text_coord(coords), normal(normal), tangent(tan) {};