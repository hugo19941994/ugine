#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "Camera.h"
#include <vector>

class World  {
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<Light>> lights;

	glm::vec3 ambientLight;
public:
	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;
	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

	const glm::vec3& getAmbient() const;
	void setAmbient(const glm::vec3& ambient);
};
