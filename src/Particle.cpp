#include "Particle.h"

Particle::Particle(const Material & mat, const glm::vec3 & vel, float spinVel, float lifetime, bool autofade) : Billboard(mat), vel(vel), spinVel(spinVel), lifetime(lifetime), autofade(autofade), remainingLifetime(lifetime) {};

float Particle::getRemainingLifetime() const
{
	return Particle::remainingLifetime;
}

void Particle::update(float deltaTime)
{
	Particle::remainingLifetime = std::max(Particle::remainingLifetime - deltaTime, 0.0f);
	setPosition(getPosition() + Particle::vel * deltaTime);
	setQuat(glm::rotate(getQuat(), Particle::spinVel * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f)));

	if (Particle::autofade) {
		glm::vec4 color = getMaterial().getColor();
		color.a = Particle::remainingLifetime / Particle::lifetime;
		getMaterial().setColor(color);
	}
}
