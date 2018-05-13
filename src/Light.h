#pragma once

#include "Entity.h"
#include "Shader.h"

class Light : public Entity {
public:
	enum class Type {
		DIRECTIONAL,
		POINT
	};

private:
	Type type;
	glm::vec3 color;
	glm::vec3 direction;
	float linearAttenuation;

public:
	Light(Type type, glm::vec3 dir);

	Type getType() const;
	void setType(Type type);
	const glm::vec3& getColor() const;
	void setColor(const glm::vec3& color);
	float getLinearAttenuation() const;
	void setLinearAttenuation(float att);
	void prepare(int index, std::shared_ptr<Shader>& shader) const;

};