#include "Camera.h"
#include "State.h"

using namespace glm;

Camera::Camera() {
}

const glm::mat4 & Camera::getProjection() const {
	return Camera::projection;
}

void Camera::setProjection(const glm::mat4 & proj) {
	projection = proj;
}

const glm::ivec4 & Camera::getViewport() const {
	return Camera::viewport;
}

void Camera::setViewport(const glm::ivec4 & vp) {
	viewport = vp;
}

const glm::vec3 & Camera::getClearColor() const {
	return Camera::clearColor;
}

void Camera::setClearColor(const glm::vec3 & color) {
	clearColor = color;
}

void Camera::prepare() {
	// For velocity buffers - Store old View & Projection matrix
	State::prevModelMatrix = mat4(State::modelMatrix);
	State::prevViewMatrix = mat4(State::viewMatrix);
	State::prevProjectionMatrix = mat4(State::projectionMatrix);

	// New matrices
	State::projectionMatrix = projection;

	State::viewMatrix = glm::rotate(glm::mat4(), -glm::angle(getQuat()), glm::axis(getQuat()));
	State::viewMatrix = glm::translate(State::viewMatrix, -getPosition());
	State::eyePos = getPosition();

	// In case a Material has disabled the depth buffer
	glDepthMask(true);

	glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	glScissor(viewport.x, viewport.y, viewport.z, viewport.w);

	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
