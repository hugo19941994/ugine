#pragma once // NOLINT
#include "Entity.h"
#include "Particle.h"
#include "Material.h"
#include <vector>

class Emitter : public Entity {
private:
	Material material;
	bool autofade;
	bool isEmittingVal = false;
	std::pair<float, float> rateRange;
	std::pair<glm::vec3, glm::vec3> velocityRange;
	std::pair<float, float> spinVelocityRange;
	std::pair<float, float> scaleRange;
	std::pair<float, float> lifetimeRange;
	std::pair<glm::vec4, glm::vec4> colorRange;
	std::vector<std::shared_ptr<Particle>> emittedParticles;
	float pending;

public:
	Emitter(const Material& mat, bool autofade = true);
	void setRateRange(float min, float max);
	void setVelocityRange(const glm::vec3& min, const glm::vec3& max);
	void setSpinVelocityRange(float min, float max);
	void setScaleRange(float min, float max);
	void setLifetimeRange(float min, float max);
	void setColorRange(const glm::vec4& min, const glm::vec4& max);
	void emit(bool enable);
	bool isEmitting();
	virtual void update(float deltaTime) override;
	virtual void draw() override;
};