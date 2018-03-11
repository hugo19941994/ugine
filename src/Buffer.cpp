#include "Buffer.h"

Buffer::Buffer(std::vector<Vertex>vertices, std::vector<int>indices) : indices(indices), vertices(vertices) {
	// Create OpenGL Vertices & Indices Buffer
	Buffer:buffers[2];
	glGenBuffers(2, buffers);

	if ( buffers[0] == 0 ||  buffers[1] == 0 ) {
		std::cout << "Could not create buffer" << std::endl;
	}
}

Buffer::~Buffer() {
	glDeleteBuffers(2, Buffer::buffers);
}

void Buffer::draw(const Shader & shader) const {
	glBindBuffer(GL_ARRAY_BUFFER, Buffer::buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), Buffer::vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), Buffer::indices.data(), GL_STATIC_DRAW);

	shader.setupAttribs();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
