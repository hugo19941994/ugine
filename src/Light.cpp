#include "Light.h"
#include "State.h"

Light::Type Light::getType() const {
	return Light::type;
}

void Light::setType(Light::Type type) {
	Light::type = type;
}

const glm::vec3& Light::getColor() const {
	return Light::color;
}

void Light::setColor(const glm::vec3& color) {
	Light::color = color;
}

float Light::getLinearAttenuation() const {
	return Light::linearAttenuation;
}

void Light::setLinearAttenuation(float att) {
	Light::linearAttenuation = att;
}

void Light::prepare(int index, std::shared_ptr<Shader>& shader) const {
	std::string colorLocS = "color[" + std::to_string(index) + ']';
	GLint colorLoc = shader->getLocation(colorLocS.c_str());
	if (colorLoc == -1) {
		std::cout << "Couldn't find color[index]";
	}
	shader->setVec3(colorLoc, Light::color);

	std::string directionLocS = "direction[" + std::to_string(index) + ']';
	GLint directionLoc = shader->getLocation(directionLocS.c_str());
	if (directionLoc == -1) {
		std::cout << "Couldn't find direction[index]";
	}

	if (type == Light::Type::DIRECTIONAL) {
		glm::vec4 m = State::viewMatrix * State::modelMatrix * glm::vec4(Light::direction, Light::type);
		shader->setVec4(directionLoc, m);
	}
	else {
		glm::vec4 m = State::viewMatrix * glm::scale(glm::rotate(glm::translate(glm::mat4(), position), glm::angle(m_quat), glm::axis(m_quat)), scale) * glm::vec4(1, 1, 1, Light::type);
		shader->setVec4(directionLoc, m);
	}

	std::string lAttLocS = "linAtt[" + std::to_string(index) + ']';
	GLint lAttLoc = shader->getLocation(lAttLocS.c_str());
	if (lAttLoc == -1) {
		std::cout << "Couldn't find linAtt[index]";
	}
	shader->setFloat(lAttLoc, linearAttenuation);
}

Light::Light(Light::Type type, glm::vec3 dir) : type(type), direction(dir) {}