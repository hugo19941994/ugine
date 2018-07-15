#include "Billboard.h"
#include <iostream>

Billboard::Billboard(const Material & mat) : material(mat) {
	std::vector<Vertex> vertex = {
		Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3()),
		Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3()),
		Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3()),
		Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3())
	};
	std::vector<int> indices = { 0, 1, 2, 0, 2, 3 };
	Billboard::buffer = std::make_shared<Buffer>(vertex, indices);

	if (mat.getTexture()) {
		// TODO: why doesn't size = mat.getTexture()->getSize() work?
		float x = mat.getTexture()->getSize().x;
		float y = mat.getTexture()->getSize().y;
		size = glm::vec2(x, y);
	} else {
		size = glm::vec2(1.0f, 1.0f);
	}
}

const Material & Billboard::getMaterial() const
{
	return Billboard::material;
}

Material & Billboard::getMaterial()
{
	return Billboard::material;
}

const glm::vec2 & Billboard::getSize() const
{
	return Billboard::size;
}

void Billboard::setSize(const glm::vec2 & size)
{
	Billboard::size = size;
}

float Billboard::getSpin() const
{
	return Billboard::spin;
}

void Billboard::setSpin(float spin)
{
	Billboard::spin = spin;
}

void Billboard::draw()
{
	glm::mat4 model = glm::transpose(State::viewMatrix);
	model[0][3] = 0;
	model[1][3] = 0;
	model[2][3] = 0;
	model[3] = glm::vec4(getPosition(), 1.0f);

	model = glm::rotate(model, glm::radians(spin), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(size, 1));

	State::modelMatrix = model;

	Billboard::material.prepare();
	Billboard::buffer->draw(*Billboard::material.getShader());
}
