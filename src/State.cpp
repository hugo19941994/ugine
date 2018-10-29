#include "State.h"

std::shared_ptr<Shader>	State::defaultShader = nullptr;
std::shared_ptr<Shader> State::AAShader = nullptr;
std::shared_ptr<Shader> State::finalShader = nullptr;
std::shared_ptr<Shader> State::velShader = nullptr;

glm::mat4 				State::projectionMatrix;
glm::mat4 				State::viewMatrix;
glm::mat4 				State::modelMatrix;

glm::mat4 State::prevModelMatrix;
glm::mat4 State::prevViewMatrix;
glm::mat4 State::prevProjectionMatrix;

std::vector<std::shared_ptr<Light>>	State::lights;
glm::vec3               State::ambient;
glm::vec3               State::eyePos;

char State::frame;
float State::jitter;
