#include "Framebuffer.h"

Framebuffer::Framebuffer(const std::shared_ptr<Texture>& colorTex,
	const std::shared_ptr<Texture>& depthTex) {
	glGenFramebuffers(1, &_fbId);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbId);

	if (colorTex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex->getId(), 0);
	}

	if (depthTex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex->getId(), 0);
	}

	_colorTex = colorTex;
	_depthTex = depthTex;
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(0, &_fbId);
}

const std::shared_ptr<Texture>& Framebuffer::getColorTexture() const {
	return _colorTex;
}

const std::shared_ptr<Texture>& Framebuffer::getDepthTexture() const {
	return _depthTex;
}

void Framebuffer::setColorTexture(std::shared_ptr<Texture> texture) {
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->getId(), 0);
	_colorTex = texture;
}

void Framebuffer::setDepthTexture(std::shared_ptr<Texture> texture) {
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getId(), 0);
	_depthTex = texture;
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbId);
}

void Framebuffer::bindScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}