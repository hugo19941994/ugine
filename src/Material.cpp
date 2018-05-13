#include "Material.h"
#include "State.h"

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
	glm::mat4 MV = view * model;
	glm::mat4 normalMat = glm::transpose(glm::inverse(MV));

	// Model View Projection matrix
	int locMVP = shader->getLocation("MVP");
	shader->setMatrix(locMVP, MVP);

	// Model View matrix 
	// transforms vertex to observer space to calculate illumination
	int locMV = shader->getLocation("MV");
	shader->setMatrix(locMV, MV);

	// Inverse of MV. For illumination as well
	int locNormalMat = shader->getLocation("normalMat");
	shader->setMatrix(locNormalMat, normalMat);

	// Number of lights
	int locNumLights = shader->getLocation("numLights");
	shader->setInt(locNumLights, State::lights.size());

	// Diffuse color of active material
	int locMatColor = shader->getLocation("matColor");
	shader->setVec4(locMatColor, Material::color);

	// Shininess of active material
	int locMatShine = shader->getLocation("matShine");
	shader->setInt(locMatShine, Material::shininess);

	// Ambient light value
	int locAmbientLight = shader->getLocation("ambientLight");
	shader->setVec3(locAmbientLight, State::ambient);

	// Texture
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

	// Lights
	for (int i = 0; i < State::lights.size(); ++i) {
		State::lights[i]->prepare(i, shader);
	}
}

const glm::vec4& Material::getColor() const {
	return Material::color;
}

void Material::setColor(const glm::vec4& color) {
	Material::color = color;
}

uint8_t Material::getShininess() const {
	return Material::shininess;
}

void Material::setShininess(uint8_t shininess) {
	Material::shininess = shininess;
}
