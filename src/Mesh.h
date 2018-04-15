#pragma once
#include "common.h"
#include "Buffer.h"
#include "State.h"
#include "Shader.h"
#include "Material.h"
#include "pugixml.hpp"

class Mesh {
private:
	std::vector<std::pair<std::shared_ptr<Buffer>, Material>> buffers;

public:
	Mesh();
	void addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material);
	size_t getNumBuffers() const;
	const std::shared_ptr<Buffer>& getBuffer(size_t index) const;
	std::shared_ptr<Buffer>& getBuffer(size_t index);
	void draw();

	static std::shared_ptr<Mesh> load(const char* filename, const std::shared_ptr<Shader>& shader = nullptr);
	static std::vector<std::string> splitString(const std::string& str, char delim);
	template <typename T> static std::vector<T> splitStringNumber(const std::string& str, char delim);
	template <typename T> static T numberFromString(const std::string& str);

	const Material& getMaterial(size_t index) const;
	Material& getMaterial(size_t index);
};
