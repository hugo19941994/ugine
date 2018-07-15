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
	bool cube;

public:
	Texture(uint32_t id, uint32_t width, uint32_t height, bool isCube);
    ~Texture();

	static std::shared_ptr<Texture>	load(const char* filename);
	uint32_t getId() const;
	const glm::ivec2& getSize() const;
	void bind(int layer = 0) const;

	static std::shared_ptr<Texture> load(
		const char* left, const char* right,
		const char* front, const char* back,
		const char* top, const char* bottom);
	bool isCube() const;

};
