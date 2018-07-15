#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define STB_IMAGE_IMPLEMENTATION

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"
#include "State.h"
#include "World.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Emitter.h"
#include "common.h"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

#define FULLSCREEN false

std::string readString(const char *filename) {
    std::ifstream f(filename, std::ios_base::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

bool init() {
    // Init GLEW
    if (glewInit() != 0) {
        std::cout << "could not initialize glew" << std::endl;
        return false;
    }

    // Enable OpenGL States
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Read Shaders
	std::string vertexShader = readString("data/vert.glsl");
	std::string fragmentShader = readString("data/frag.glsl");
	Shader shader(vertexShader, fragmentShader);
    shader.use();

	State::defaultShader = std::make_shared<Shader>(shader);

	return true;
}

int main() {
    // Init GLEW
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "could not initalize glfw" << std::endl;
        return false;
    }

    // Create Window
    GLFWwindow *window = glfwCreateWindow(800, 600, "U-gine",
                                          FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "could not create glfw window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

	if (!init()) {
		return -1;
	}

    atexit(glfwTerminate);

	// Create World
	std::shared_ptr<World> world = std::make_shared<World>();

	// Create Camera
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->setClearColor(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	world->addEntity(camera);

	// Skybox
	std::shared_ptr<Mesh> meshSkybox = Mesh::load("data/skybox.msh.xml");
	std::shared_ptr<Model> modelSkybox = std::make_shared<Model>(meshSkybox);
	modelSkybox->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	world->addEntity(modelSkybox);

	// Teapot
	std::shared_ptr<Mesh> meshTeapot = Mesh::load("data/teapot_reflect.msh.xml");
	std::shared_ptr<Model> modelTeapot = std::make_shared<Model>(meshTeapot);
	modelTeapot->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	modelTeapot->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
	world->addEntity(modelTeapot);

	// Monkey
	std::shared_ptr<Mesh> meshSuzanne = Mesh::load("data/suzanne_refract.msh.xml");
	std::shared_ptr<Model> modelSuzanne = std::make_shared<Model>(meshSuzanne);
	modelSuzanne->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	modelSuzanne->setPosition(glm::vec3(-2.0f, 0.0f, 2.0f));
	world->addEntity(modelSuzanne);

	// Cube
	std::shared_ptr<Mesh> meshCube = Mesh::load("data/cube.msh.xml");
	std::shared_ptr<Model> modelCube = std::make_shared<Model>(meshCube);
	modelCube->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	modelCube->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	world->addEntity(modelCube);

	// Lights and ambient
	world->setAmbient(glm::vec3(1.0f, 1.0f, 1.0f));

	std::shared_ptr<Light> pLight = std::make_shared<Light>(Light::Type::POINT, glm::vec3(0, 0, 0));
	pLight->setColor(glm::vec3(0.5, 0.5, 0.5));
	pLight->setLinearAttenuation(0.2);
	world->addEntity(pLight);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double lastMX, lastMY;
	glfwGetCursorPos(window, &lastMX, &lastMY);

    auto lastTime = static_cast<float>(glfwGetTime());
	float accumulatedTime = 0.0f;
    while (glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0) {
        // Update delta time
        auto newTime = static_cast<float>(glfwGetTime());
        float deltaTime = newTime - lastTime;
        lastTime = newTime;
		accumulatedTime += deltaTime;

        // Get updated screen size
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        int frameBufferWidth, frameBufferHeight;
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

        // Report screen size
        std::stringstream ss;
        ss << screenWidth << "x" << screenHeight;
        glfwSetWindowTitle(window, ss.str().c_str());

        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1
        // unit <-> 100 units

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		float speedMX = static_cast<int>(mouseX - lastMX);
		float speedMY = static_cast<int>(mouseY - lastMY);
		lastMX = mouseX;
		lastMY = mouseY;

		int up = glfwGetKey(window, GLFW_KEY_W);
		int left = glfwGetKey(window, GLFW_KEY_A);
		int down = glfwGetKey(window, GLFW_KEY_S);
		int right = glfwGetKey(window, GLFW_KEY_D);
		glm::vec3 pos = camera->getPosition();
		
		for (int i = 0; i < world->getNumEntities(); ++i) {
			std::shared_ptr<Camera> isCamera = std::dynamic_pointer_cast<Camera>(world->getEntity(i));

			if (!isCamera) {}
			else {
				/*
				camera->setEuler(glm::vec3(camera->getEuler().x - speedMY, camera->getEuler().y - speedMX, 0));

				if (up == GLFW_PRESS) {
					camera->move(glm::vec3(0, 0, -deltaTime * 2));
				}
				if (down == GLFW_PRESS) {
					camera->move(glm::vec3(0, 0, deltaTime * 2));
				}
				if (left == GLFW_PRESS) {
					camera->move(glm::vec3(-deltaTime * 2, 0, 0));
				}
				if (right == GLFW_PRESS) {
					camera->move(glm::vec3(deltaTime * 2, 0, 0));
				}
				*/
			}
		}

		camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		camera->setEuler(glm::vec3(-10, accumulatedTime * 25, 0));
		camera->move(glm::vec3(0.0f, 0.0f, 5.0f));

		modelSkybox->setPosition(camera->getPosition());
		pLight->setPosition(camera->getPosition() + glm::vec3(0.0f, 0.0f, -0.0001f));

        camera->setProjection(glm::perspective(
            glm::radians(90.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.1f, 100.0f));
		camera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));
		camera->prepare();

		world->update(deltaTime);
		world->draw();

        // Update swap chain & process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
