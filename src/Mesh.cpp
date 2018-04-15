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

			// Texture
			pugi::xml_node materialNode = bufferNode.child("material");
			std::string textureStr = materialNode.child("texture").text().as_string();

			// Indices
			std::string indicesNode = bufferNode.child("indices").text().as_string();
			std::vector<int> indices = splitStringNumber<int>(indicesNode, ',');

			// Vertex coords
			std::string coordsNode = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitStringNumber<float>(coordsNode, ',');

			// Texture coords
			std::string texcoordsNode = bufferNode.child("texcoords").text().as_string();
			std::vector<float> texcoords = splitStringNumber<float>(texcoordsNode, ',');

			std::vector<Vertex> vertices;
			Vertex v();
			for (int i = 0; i < coords.size()/3; i++) {
				Vertex vertex(
					coords.at(i * 3),
					coords.at(i * 3 + 1),
					coords.at(i * 3 + 2),
					texcoords.at(i * 2),
					texcoords.at(i * 2 + 1)
				);

				vertices.push_back(vertex);
			}

			Buffer buffer(vertices, indices);

			mesh.addBuffer(std::make_shared<Buffer>(buffer), Material(Texture::load(textureStr.c_str())));
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
