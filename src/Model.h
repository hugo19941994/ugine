#pragma once // NOLINT
#include "common.h"
#include "Mesh.h"
#include "Entity.h"


class Model : public Entity {
private:
	std::shared_ptr<Mesh> mesh;

  public:
	  Model(const std::shared_ptr<Mesh>& mesh);
	  virtual void draw() override;
};
