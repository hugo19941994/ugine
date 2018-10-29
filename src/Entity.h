#pragma once
#include "common.h"

class Entity {
protected:
	glm::vec3 position, scale, m_euler, prevPosition;
	glm::quat m_quat;

public:
	Entity();
	virtual ~Entity() {}

	const glm::vec3& getPosition() const;
	void setPosition(const glm::vec3& pos);
	const glm::vec3& getScale() const;
	void setScale(const glm::vec3& scale);
	const glm::vec3& getEuler() const { return m_euler; }
	const void setEuler(const glm::vec3& rot) { m_euler = rot; m_quat = glm::quat(glm::radians(rot)); }
	const glm::quat& getQuat() const { return m_quat; }
	const void setQuat(const glm::quat& rot) { m_quat = rot; m_euler = glm::degrees(glm::eulerAngles(m_quat)); }

	void move(const glm::vec3& vec);	// Ver en las diapositivas

	virtual void update(float deltaTime) {}
	virtual void draw() {}
};
