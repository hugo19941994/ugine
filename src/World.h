#pragma once

#include "Entity.h"
#include "Mesh.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

class World  {
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<Light>> lights;

	glm::vec3 ambientLight;

	std::shared_ptr<Framebuffer> renderFramebuffer;
	std::shared_ptr<Texture> currentTexture;

	std::shared_ptr<Framebuffer> AAframebuffer;
	std::shared_ptr<Texture> historyAATexture;

	std::shared_ptr<Framebuffer> velocityFramebuffer;
	std::shared_ptr<Texture> velocityTexture;
public:
	World();
	void addEntity(const std::shared_ptr<Entity>& entity);
	void removeEntity(const std::shared_ptr<Entity>& entity);
	size_t getNumEntities() const;
	const std::shared_ptr<Entity>& getEntity(size_t index) const;
	std::shared_ptr<Entity>& getEntity(size_t index);
	void update(float deltaTime);
	void draw();

	const glm::vec3& getAmbient() const;
	void setAmbient(const glm::vec3& ambient);

	/*
	void setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer);
	const std::shared_ptr<const Framebuffer> getFramebuffer() const;
	std::shared_ptr<Framebuffer> getFramebuffer();
	*/

	void postprocess();
	void finalImage();
	std::shared_ptr<Texture> currentAATexture;

	void velocity();
};
