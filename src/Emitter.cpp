#include "Emitter.h"

Emitter::Emitter(const Material & mat, bool autofade) : material(mat), autofade(autofade) {}

void Emitter::setRateRange(float min, float max) {
	Emitter::rateRange = std::pair<float, float>(min, max);
}

void Emitter::setVelocityRange(const glm::vec3 & min, const glm::vec3 & max) {
	Emitter::velocityRange = std::pair<glm::vec3, glm::vec3>(min, max);
}

void Emitter::setSpinVelocityRange(float min, float max) {
	Emitter::spinVelocityRange = std::pair<float, float>(min, max);
}

void Emitter::setScaleRange(float min, float max) {
	Emitter::scaleRange = std::pair<float, float>(min, max);
}

void Emitter::setLifetimeRange(float min, float max) {
	Emitter::lifetimeRange = std::pair<float, float>(min, max);
}

void Emitter::setColorRange(const glm::vec4 & min, const glm::vec4 & max) {
	Emitter::colorRange = std::pair<glm::vec4, glm::vec4>(min, max);
}

void Emitter::emit(bool enable) {
	Emitter::isEmittingVal = enable;
}

bool Emitter::isEmitting() {
	return Emitter::isEmittingVal;
}

void Emitter::update(float deltaTime) {
	if (Emitter::isEmittingVal) {
		float particlesToEmit = glm::linearRand(rateRange.first, rateRange.second) * deltaTime + pending;
		pending = particlesToEmit - (int)particlesToEmit;

		for (int i = 0; i < particlesToEmit; i++) {
			glm::vec4 randColor = glm::linearRand(colorRange.first, colorRange.second);
			glm::vec3 randVelocity = glm::linearRand(velocityRange.first, velocityRange.second);
			float randSpinVelocity = glm::linearRand(spinVelocityRange.first, spinVelocityRange.second);
			float randLifetime = glm::linearRand(lifetimeRange.first, lifetimeRange.second);
			float randScale = glm::linearRand(scaleRange.first, scaleRange.second);

			material.setColor(randColor);
			std::shared_ptr<Particle> p = std::make_shared<Particle>(material, randVelocity, randSpinVelocity, randLifetime, autofade);
			p->setSize(p->getSize() * randScale * 0.5f);
			p->setPosition(position);
			emittedParticles.push_back(p);
		}

		for (auto p : emittedParticles) {
			p->update(deltaTime);
		}

		auto it = emittedParticles.begin();
		while (it != emittedParticles.end()) {
			if ((*it)->getRemainingLifetime() <= 0) it = emittedParticles.erase(it);
			else ++it;
		}
	}
}

void Emitter::draw() {
	for (auto p : emittedParticles) {
		p->draw();
	}
}
