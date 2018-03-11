#pragma once
#include "common.h"
#include "Shader.h"
#include "Vertex.h"
#include "string"
#include <iostream>
#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Buffer {
private:
	uint32_t buffers[2];
	std::vector<Vertex>vertices;
	std::vector<int>indices;

public:
	Buffer(std::vector<Vertex>vertices, std::vector<int>indices);
	~Buffer();
	void draw(const Shader& shader) const;
};
