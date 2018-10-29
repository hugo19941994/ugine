#include "World.h"
#include "State.h"

World::World() {
	currentTexture = std::make_shared<Texture>(1920, 1080, false);
	currentAATexture = std::make_shared<Texture>(1920, 1080, false);
	historyAATexture = std::make_shared<Texture>(1920, 1080, false);
	std::shared_ptr<Texture> depthBuffer = std::make_shared<Texture>(1920, 1080, true);

	renderFramebuffer = std::make_shared<Framebuffer>(currentTexture, depthBuffer); // Render to texture
	AAframebuffer = std::make_shared<Framebuffer>(currentAATexture, nullptr); // Apply AA

	velocityTexture = std::make_shared<Texture>(1920, 1080, GL_RG16F);
	velocityFramebuffer = std::make_shared<Framebuffer>(velocityTexture, nullptr);
}

void World::addEntity(const std::shared_ptr<Entity>& entity) {
	std::shared_ptr<Camera> isCamera = std::dynamic_pointer_cast<Camera>(entity);
	if (isCamera) {
		World::cameras.push_back(isCamera);
	}

	std::shared_ptr<Light> isLight = std::dynamic_pointer_cast<Light>(entity);
	if (isLight) {
		World::lights.push_back(isLight);
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

	std::shared_ptr<Light> isLight = std::dynamic_pointer_cast<Light>(entity);

	if (isLight) {
		auto lightIt = lights.begin();
		while (*lightIt != isLight && lightIt != lights.end()) {
			lightIt++;
		}

		World::lights.erase(lightIt);
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

void World::postprocess() {
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(quadVAO);

	int locMotionTexture = State::AAShader->getLocation("motionTexture");
	State::AAShader->setInt(locMotionTexture, 10);
	if (velocityTexture != nullptr) velocityTexture->bind(10);

	int locCurrentTexture = State::AAShader->getLocation("currentTexture");
	State::AAShader->setInt(locCurrentTexture, 11);
	if (currentTexture != nullptr) renderFramebuffer->getColorTexture()->bind(11);

	int locHistoryAATexture = State::AAShader->getLocation("historyAATexture");
	State::AAShader->setInt(locHistoryAATexture, 12);
	if (historyAATexture != nullptr) historyAATexture->bind(12);

	// Image rendered
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Current antialiased image is now the history
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 1920, 1080);
}

void World::finalImage() {
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(quadVAO);

	int locCurrentTexture = State::finalShader->getLocation("finalTexture");
	State::finalShader->setInt(locCurrentTexture, 15);
	if (currentTexture != nullptr) AAframebuffer->getColorTexture()->bind(15);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void World::velocity() {
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(quadVAO);

	int locDepthTexture = State::velShader->getLocation("depthTexture");
	State::velShader->setInt(locDepthTexture, 9);
	if (renderFramebuffer != nullptr) renderFramebuffer->getDepthTexture()->bind(9);

	int locInvVP = State::velShader->getLocation("VP");
	glm::mat4 VP = State::projectionMatrix * State::viewMatrix;
	State::velShader->setMatrix(locInvVP, glm::inverse(VP));

	int locPrevInvVP = State::velShader->getLocation("PrevVP");
	glm::mat4 prevVP = State::prevProjectionMatrix * State::prevViewMatrix;
	State::velShader->setMatrix(locPrevInvVP, prevVP);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void World::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render world (jittered)
	State::defaultShader->use();
	renderFramebuffer->bind();
	Framebuffer::bindScreen();

	// Draws to texture
	State::ambient = World::ambientLight;
	State::lights = World::lights;

	for (auto i : cameras) {
		i->prepare();

		for (auto j : entities) {
			j->draw();
		}
	}

	// Velocity buffer
	State::velShader->use();
	velocityFramebuffer->bind();
	velocity();

	// Temporal AA
	State::AAShader->use();
	AAframebuffer->bind();
	postprocess();

	// Display final image
	State::finalShader->use();
	Framebuffer::bindScreen();
	finalImage();

}

const glm::vec3& World::getAmbient() const {
	return World::ambientLight;
}

void World::setAmbient(const glm::vec3& ambient) {
	World::ambientLight = ambient;
}

/*
void World::setFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) {
	_framebuffer = framebuffer;
}

const std::shared_ptr<const Framebuffer> World::getFramebuffer() const {
	return _framebuffer;
}

std::shared_ptr<Framebuffer> World::getFramebuffer() {
	return _framebuffer;
}
*/
