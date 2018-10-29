#include "Texture.h"

Texture::Texture(uint32_t id, uint32_t width, uint32_t height, bool cube) : id(id), width(width), height(height), cube(cube), depth(false) {}

Texture::Texture(uint16_t width, uint16_t height, bool isDepth) : cube(false), depth(isDepth), width(width), height(height) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, isDepth ? GL_DEPTH_COMPONENT24 : GL_RGBA, width, height, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	GLuint clearColor[4] = { 0, 0, 0, 0 };
	glClearTexImage(id, 0, GL_BGRA, GL_UNSIGNED_BYTE, &clearColor);

}

// TODO: 
Texture::Texture(uint16_t width, uint16_t height, GLint format) {
	cube = false;
	depth = false;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

Texture::~Texture() {
	if (Texture::id != 0) {
		glDeleteTextures(1, &id);
	} else {
		std::cout << "Couldn't delete texture" << std::endl;
	}
}

unsigned char * load_img(char const *filename, int *width, int *height, bool flip) {
	// OpenGL expects images bottom to top
	stbi_set_flip_vertically_on_load(flip);

	unsigned char* img = stbi_load(filename, width, height, nullptr, 4);

	if (img == nullptr) {
		// Retry with /data
		char dataFilename[1024];
		strcpy_s(dataFilename, "data/");
		strcat_s(dataFilename, filename);
		img = stbi_load(dataFilename, width, height, nullptr, 4);
		if (img == nullptr) {
			std::cout << "Error loading texture" << std::endl;
			return nullptr;
		}
	}

	return img;
}

std::shared_ptr<Texture> Texture::load(const char * filename) {
	int width, height;

	unsigned char* img = load_img(filename, &width, &height, true);

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

	return std::make_shared<Texture>(texId, width, height, false);
}

uint32_t Texture::getId() const {
	return Texture::id;
}

const glm::ivec2 & Texture::getSize() const {
	return glm::ivec2(Texture::width, Texture::height);
}

void Texture::bind(int layer) const {
	glActiveTexture(GL_TEXTURE0 + layer);
	if (Texture::cube) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	} else {
		glBindTexture(GL_TEXTURE_2D, id);
	}
}

std::shared_ptr<Texture> Texture::load(const char * left, const char * right, const char * front, const char * back, const char * top, const char * bottom) {
	int width, height;

	// OpenGL expects images bottom to top
	stbi_set_flip_vertically_on_load(false);

	unsigned char* imgLeft = load_img(left, &width, &height, false);
	unsigned char* imgRight = load_img(right, &width, &height, false);
	unsigned char* imgFront = load_img(front, &width, &height, false);
	unsigned char* imgBack = load_img(back, &width, &height, false);
	unsigned char* imgTop = load_img(top, &width, &height, false);
	unsigned char* imgBottom = load_img(bottom, &width, &height, false);

	if (imgLeft == nullptr || imgRight == nullptr || imgFront == nullptr
		|| imgBack == nullptr || imgTop == nullptr || imgBottom == nullptr) {
		std::cout << "Error loading texture" << std::endl;
		return nullptr;
	}

	GLuint texId;
	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgLeft);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgRight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFront);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBack);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgTop);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBottom);

	stbi_image_free(imgLeft);
	stbi_image_free(imgRight);
	stbi_image_free(imgFront);
	stbi_image_free(imgBack);
	stbi_image_free(imgTop);
	stbi_image_free(imgBottom);

	return std::make_shared<Texture>(texId, width, height, true);
}

bool Texture::isCube() const {
	return Texture::cube;
}

bool Texture::isDepth() const {
	return depth;
}
