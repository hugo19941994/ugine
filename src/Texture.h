#pragma once

#include "common.h"
#include "stb_image.h"
#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include <iostream>

class Texture  {
private:
	uint32_t id, width, height;
	std::shared_ptr<void*> img;

public:
	Texture(uint32_t id, uint32_t width, uint32_t height);
    ~Texture();

	static std::shared_ptr<Texture>	load(const char* filename);
	uint32_t getId() const;
	const glm::ivec2& getSize() const;
	void bind() const;
};
