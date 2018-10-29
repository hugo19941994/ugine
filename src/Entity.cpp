#include "Entity.h"

Entity::Entity() {}

const glm::vec3 & Entity::getPosition() const
{
	return Entity::position;
}

void Entity::setPosition(const glm::vec3 & pos)
{
	Entity::prevPosition = position;
	Entity::position = pos;
}

const glm::vec3 & Entity::getScale() const
{
	return Entity::scale;
}

void Entity::setScale(const glm::vec3 & scale)
{
	Entity::scale = scale;
}

void Entity::move(const glm::vec3 & vec)
{
	Entity::position += m_quat * vec;
}

