#pragma once

#include "common.h"
#include "Texture.h"
#include "Shader.h"
#include "State.h"

class Material  {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Texture> texture;
public:
	Material(const std::shared_ptr<Texture>& tex = nullptr,
		const std::shared_ptr<Shader>& shader = nullptr);
	const std::shared_ptr<Shader>& getShader() const;
	std::shared_ptr<Shader>& getShader();
	void setShader(const std::shared_ptr<Shader>& shader);
	const std::shared_ptr<Texture>& getTexture() const;
	void setTexture(const std::shared_ptr<Texture>& tex);
	void prepare();

};
