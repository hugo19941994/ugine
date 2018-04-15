#include "World.h"

void World::addEntity(const std::shared_ptr<Entity>& entity) {
	std::shared_ptr<Camera> isCamera = std::dynamic_pointer_cast<Camera>(entity);
	if (isCamera) {
		World::cameras.push_back(isCamera);
	}
	World::entities.push_back(entity);
}

void World::removeEntity(const std::shared_ptr<Entity>& entity) {
	std::shared_ptr<Camera> isCamera = std::dynamic_pointer_cast<Camera>(entity);

	if (isCamera) {
		auto cameraIt = cameras.begin();
		while (*cameraIt != isCamera && cameraIt!= cameras.end() ) {
			cameraIt++;
		}

		World::cameras.erase(cameraIt);
	}

	auto entityIt = entities.begin();
	while (*entityIt != entity && entityIt != entities.end()) {
		entityIt++;
	}

	World::entities.erase(entityIt);
}

size_t World::getNumEntities() const {
	return entities.size();
}

const std::shared_ptr<Entity>& World::getEntity(size_t index) const {
	return entities.at(index);
}

std::shared_ptr<Entity>& World::getEntity(size_t index) {
	return entities.at(index);
}

void World::update(float deltaTime) {
	for (auto i : entities) {
		i->update(deltaTime);
	}
}

void World::draw() {
	for (auto i : cameras) {
		i->prepare();

		for (auto j : entities) {
			j->draw();
		}
	}
	
}
