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
	if (lighting) {
		for (int i = 0; i < State::lights.size(); ++i) {
			State::lights[i]->prepare(i, shader);
		}
	}

	// Blend Mode
	if (blendMode == BlendMode::ALPHA) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	else if (blendMode == BlendMode::ADD) glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else if (blendMode == BlendMode::MUL) glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	// Culling
	if (culling) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);

	// Depth write
	if (depthWrite) glDepthMask(GL_TRUE);
	else glDepthMask(GL_FALSE);
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

BlendMode Material::getBlendMode() const
{
	return BlendMode();
}

void Material::setBlendMode(BlendMode blendMode) {
	Material::blendMode = blendMode;
}

bool Material::getLighting() const {
	return Material::lighting;
}

void Material::setLighting(bool enable) {
	Material::lighting = enable;
}

bool Material::getCulling() const {
	return Material::culling;
}

void Material::setCulling(bool enable) {
	Material::culling = enable;
}

bool Material::getDepthWrite() const {
	return Material::depthWrite;
}

void Material::setDepthWrite(bool enable) {
	Material::depthWrite = enable;
}
