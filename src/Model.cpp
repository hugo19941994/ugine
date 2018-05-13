#include "Model.h"
#include "State.h"

State STATE;

Model::Model(const std::shared_ptr<Mesh>& mesh)
{
	Model::mesh = mesh;
}

void Model::draw()
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 rot = glm::rotate(trans, glm::angle(m_quat), glm::axis(m_quat));
	State::modelMatrix = glm::scale(rot, scale);

	mesh->draw();
}
