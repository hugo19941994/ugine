#pragma once
#include "Mesh.h"

Mesh::Mesh() {}

void Mesh::addBuffer(const std::shared_ptr<Buffer>& buffer, const Material& material) {
	Mesh::buffers.push_back(std::pair<std::shared_ptr<Buffer>, Material>(buffer, material));
}

size_t Mesh::getNumBuffers() const {
	return Mesh::buffers.size();
}

const std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) const {
	return buffers.at(index).first;
}

std::shared_ptr<Buffer>& Mesh::getBuffer(size_t index) {
	return buffers.at(index).first;
}

void Mesh::draw() {
	for (auto pair : Mesh::buffers) {
		auto buffer = pair.first;
		auto material = pair.second;

		material.prepare();
		buffer->draw(*(material.getShader()));
	}
}

std::shared_ptr<Mesh> Mesh::load(const char * filename, const std::shared_ptr<Shader>& shader) {
	Mesh mesh;

	std::shared_ptr<Shader> s = shader;
	if (s == nullptr) {
		s = State::defaultShader;
	}

	pugi::xml_document doc; 
	pugi::xml_parse_result result = doc.load_file(filename); 

	if (result) {
		// Cargado correctamente, podemos analizar su contenido ... 
		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");
		for (pugi::xml_node bufferNode = buffersNode.child("buffer");
			bufferNode;
			bufferNode = bufferNode.next_sibling("buffer")) {
			// Iteramos por todos los buffers 

			pugi::xml_node materialNode = bufferNode.child("material");
			Material material;

			// Texture
			if (materialNode.child("texture")) {
				std::string texturesStr = materialNode.child("texture").text().as_string();
				std::vector<std::string>textures = splitString(texturesStr, ',');
				if (textures.size() == 1) {
					material.setTexture(Texture::load(textures.at(0).c_str()));
				}
				else if (textures.size() == 6) {
					material.setTexture(Texture::load(textures.at(0).c_str(), textures.at(1).c_str(), textures.at(2).c_str(),
						textures.at(3).c_str(), textures.at(4).c_str(), textures.at(5).c_str()));
				}
				else {
					std::cout << "Error loading texture(s). Pass 6 paths for a cubemap or 1 for a plain texture" << std::endl;
				}
			}

			// Normal Texture
			if (materialNode.child("normal_texture")) {
				std::string texturesStr = materialNode.child("normal_texture").text().as_string();
				std::vector<std::string>textures = splitString(texturesStr, ',');
				if (textures.size() == 1) {
					material.setNormalTexture(Texture::load(textures.at(0).c_str()));
				}
				else {
					std::cout << "Error loading normal texture(s)" << std::endl;
				}
			}

			// Reflection Texture
			if (materialNode.child("reflect_texture")) {
				std::string texturesStr = materialNode.child("reflect_texture").text().as_string();
				std::vector<std::string>textures = splitString(texturesStr, ',');
				if (textures.size() == 6) {
					material.setReflectionTexture(Texture::load(textures.at(0).c_str(), textures.at(1).c_str(), textures.at(2).c_str(),
						textures.at(3).c_str(), textures.at(4).c_str(), textures.at(5).c_str()));
				}
				else {
					std::cout << "Error loading reflection texture(s)" << std::endl;
				}
			}

			// Refraction Texture
			if (materialNode.child("refract_texture")) {
				std::string texturesStr = materialNode.child("refract_texture").text().as_string();
				std::vector<std::string>textures = splitString(texturesStr, ',');
				if (textures.size() == 6) {
					material.setRefractionTexture(Texture::load(textures.at(0).c_str(), textures.at(1).c_str(), textures.at(2).c_str(),
						textures.at(3).c_str(), textures.at(4).c_str(), textures.at(5).c_str()));
				}
				else {
					std::cout << "Error loading refraction texture(s)" << std::endl;
				}
			}

			// Refraction coefficient
			if (materialNode.child("refract_coef")) {
				material.setRefractionCoef(materialNode.child("refract_coef").text().as_float());
			}

			// Culling
			if (materialNode.child("culling")) {
				material.setCulling(materialNode.child("culling").text().as_bool());
			}

			// Depth write
			if (materialNode.child("depthwrite")) {
				material.setDepthWrite(materialNode.child("depthWrite").text().as_bool());
			}

			// Blend Mode
			if (materialNode.child("blend")) {
				std::string blendModeStr = materialNode.child("blend").text().as_string();
				if (blendModeStr == "alpha") material.setBlendMode(BlendMode::ALPHA);
				else if (blendModeStr == "add") material.setBlendMode(BlendMode::ADD);
				else if (blendModeStr == "mul") material.setBlendMode(BlendMode::MUL);
				else std::cout << "Error setting blend mode" << std::endl;
			}

			// Color
			if (materialNode.child("color")) {
				std::string colorStr = materialNode.child("color").text().as_string();
				std::vector<float> colors = splitStringNumber<float>(colorStr, ',');
				material.setColor(glm::vec4(
					colors.at(0),
					colors.at(1),
					colors.at(2),
					colors.at(3)
				));
			}

			// Shininess
			if (materialNode.child("shininess")) {
				int shininessInt = materialNode.child("shininess").text().as_int();
				material.setShininess(shininessInt);
			}

			// Tangents
			std::vector<float> tangents;
			if (bufferNode.child("tangents")) {
				std::string tangentsNode = bufferNode.child("tangents").text().as_string();
				tangents = splitStringNumber<float>(tangentsNode, ',');
			}

			// Indices
			std::string indicesNode = bufferNode.child("indices").text().as_string();
			std::vector<int> indices = splitStringNumber<int>(indicesNode, ',');

			// Vertex coords
			std::string coordsNode = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitStringNumber<float>(coordsNode, ',');

			// Texture coords
			std::vector<float> texcoords;
			if (bufferNode.child("texcoords")) {
				std::string texcoordsNode = bufferNode.child("texcoords").text().as_string();
				texcoords = splitStringNumber<float>(texcoordsNode, ',');
			}

			// Normals
			std::vector<float> normals;
			if (bufferNode.child("normals")) {
				std::string normalsNode = bufferNode.child("normals").text().as_string();
				normals = splitStringNumber<float>(normalsNode, ',');
			}

			std::vector<Vertex> vertices;
			for (int i = 0; i < coords.size()/3; i++) {
				Vertex vertex = Vertex();

				vertex.position = glm::vec3(coords.at(i * 3), coords.at(i * 3 + 1), coords.at(i * 3 + 2));

				if (normals.size() > 0) {
					vertex.normal = glm::vec3(normals.at(i * 3), normals.at(i * 3 + 1), normals.at(i * 3 + 2));
				} else {
					vertex.normal = glm::vec3();
				}

				if (texcoords.size() > 0) {
					vertex.text_coord = glm::vec2(texcoords.at(i * 2), texcoords.at(i * 2 + 1));
				} else {
					vertex.text_coord = glm::vec2();
				}

				if (tangents.size() > 0) {
					vertex.tangent = glm::vec3(tangents.at(i * 3), tangents.at(i * 3 + 1), tangents.at(i * 3 + 2));
				} else {
					vertex.tangent = glm::vec3();
				}

				vertices.push_back(vertex);
			}

			Buffer buffer(vertices, indices);

			mesh.addBuffer(std::make_shared<Buffer>(buffer), material);
		}
	} else {  
		// No se ha podido cargar
		std::cout << result.description() << std::endl;
		return nullptr;
	}

	return std::make_shared<Mesh>(mesh);
}

std::vector<std::string> Mesh::splitString(const std::string& str, char delim) {
	std::vector<std::string> elems;
	std::stringstream sstream(str);
	std::string item;
	if (str != "") {
		while (std::getline(sstream, item, delim)) {
			elems.push_back(item);
		}
	}
	return elems;
}

template <typename T> std::vector<T> Mesh::splitStringNumber(const std::string& str, char delim) {
	std::vector<T> elems;
	std::stringstream sstream(str);
	std::string item;
	if (str != "") {
		while (std::getline(sstream, item, delim)) {
			elems.push_back(numberFromString<T>(item));
		}
	}
	return elems;
}

template <typename T> T Mesh::numberFromString(const std::string& str) {
	T number;
	std::istringstream stream(str);
	stream >> number;
	return number;
}

const Material & Mesh::getMaterial(size_t index) const {
	return buffers.at(index).second;
}

Material & Mesh::getMaterial(size_t index) {
	return buffers.at(index).second;
}
