#include "Material.h"

Material::Material(const std::shared_ptr<Texture>& tex, const std::shared_ptr<Shader>& shader) : shader(shader), texture(tex) {}

const std::shared_ptr<Shader>& Material::getShader() const {
	if (Material::shader == nullptr) {
		return State::defaultShader;
	}

	return Material::shader;
}

std::shared_ptr<Shader>& Material::getShader() {
	return Material::shader == nullptr ? State::defaultShader : Material::shader;
}

void Material::setShader(const std::shared_ptr<Shader>& shader) {
	Material::shader = shader;
}

const std::shared_ptr<Texture>& Material::getTexture() const {
	return Material::texture;
}

void Material::setTexture(const std::shared_ptr<Texture>& tex) {
	Material::texture = tex;
}

void Material::prepare() {
	auto shader = getShader();
	shader->use();

	glm::mat4 model = State::modelMatrix;
	glm::mat4 view = State::viewMatrix;
	glm::mat4 projection = State::projectionMatrix;

	glm::mat4 MVP = projection * view * model;

	int locMVP = shader->getLocation("MVP");
	shader->setMatrix(locMVP, MVP);

	int locTS = shader->getLocation("texSampler");
	shader->setInt(locTS, 0);

	int isTexture = shader->getLocation("isTexturized");

	auto texture = getTexture();

	if (texture != nullptr) {
		shader->setInt(isTexture, true);
		texture->bind();
	}
	else {
		shader->setInt(isTexture, false);
	}

}
