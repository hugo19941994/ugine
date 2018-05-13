#pragma once

#include "Entity.h"


class Camera : public Entity {
private:
	glm::mat4 projection;
	glm::ivec4 viewport;
	glm::vec3 clearColor;
public:
	const glm::mat4& getProjection() const;
	void setProjection(const glm::mat4& proj);
	const glm::ivec4& getViewport() const;
	void setViewport(const glm::ivec4& vp);
	const glm::vec3& getClearColor() const;
	void setClearColor(const glm::vec3& color);

	void prepare();

	// virtual void draw() override;
	// virtual void	update(float deltaTime) {}
};