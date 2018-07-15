#pragma once // NOLINT
#include "Buffer.h"
#include "Entity.h"
#include "Material.h"
#include "State.h"
#include <vector>

class Billboard : public Entity {
  private:
	  Material material;
	  glm::vec2 size;
	  float spin;
	  std::shared_ptr<Buffer> buffer;

  public:
	Billboard(const Material& mat);
	const Material&	getMaterial() const;
	Material& getMaterial();
	const glm::vec2& getSize() const;
	void setSize(const glm::vec2& size);
	float getSpin() const;
	void setSpin(float spin);
	virtual void draw() override;
};
