#pragma once // NOLINT
#include "Billboard.h"
#include <algorithm>

class Particle : public Billboard {
  private:
	  glm::vec3 vel;
	  float spinVel = 0.0f;
	  float lifetime = 0.0f;
	  bool autofade = true;
	  float remainingLifetime = 0.0f;

  public:
	  Particle(const Material& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade);
	  float getRemainingLifetime() const;
	  virtual void update(float deltaTime) override;
};
