#include "Texture.h"

Texture::Texture(uint32_t id, uint32_t width, uint32_t height) : id(id), width(width), height(height) {}

Texture::~Texture() {
	if (Texture::id != 0) {
		glDeleteTextures(1, &id);
	} else {
		std::cout << "Couldn't delete texture" << std::endl;
	}
}

std::shared_ptr<Texture> Texture::load(const char * filename) {
	int width, height;

	// OpenGL expects images bottom to top
	stbi_set_flip_vertically_on_load(true);

	unsigned char* img = stbi_load(filename, &width, &height, nullptr, 4);

	if (img == nullptr) {
		std::cout << "Error loading texture" << std::endl;
		return nullptr;
	}

	GLuint texId;
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img);

	return std::make_shared<Texture>(texId, width, height);
}

uint32_t Texture::getId() const {
	return Texture::id;
}

const glm::ivec2 & Texture::getSize() const {
	return glm::ivec2(Texture::width, Texture::height);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}
