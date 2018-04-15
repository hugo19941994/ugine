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

	// Read Shaders
	std::string vertexShader = readString("data/shader.vert");
	std::string fragmentShader = readString("data/shader.frag");
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
	camera->setPosition(glm::vec3(3, 5, 10));
	camera->setQuat(glm::rotate(glm::quat(), glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	camera->setClearColor(glm::vec3(0.4, 0.753, 0.898));

	world->addEntity(camera);

	std::shared_ptr<Mesh> mesh = Mesh::load("data/asian_town.msh.xml");

	std::shared_ptr<Model> model = std::make_shared<Model>(mesh);
	model->setPosition(glm::vec3());
	model->setQuat(glm::quat());
	model->setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	world->addEntity(model);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double lastMX, lastMY;	glfwGetCursorPos(window, &lastMX, &lastMY);

    auto lastTime = static_cast<float>(glfwGetTime());
    while (glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0) {
        // Update delta time
        auto newTime = static_cast<float>(glfwGetTime());
        float deltaTime = newTime - lastTime;
        lastTime = newTime;

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

		double mouseX, mouseY;		glfwGetCursorPos(window, &mouseX, &mouseY);
		float speedMX = static_cast<int>(mouseX - lastMX);
		float speedMY = static_cast<int>(mouseY - lastMY);		lastMX = mouseX;
		lastMY = mouseY;
		int up = glfwGetKey(window, GLFW_KEY_W);
		std::cout << up << std::endl;
		int left = glfwGetKey(window, GLFW_KEY_A);
		int down = glfwGetKey(window, GLFW_KEY_S);
		int right = glfwGetKey(window, GLFW_KEY_D);
		glm::vec3 pos = camera->getPosition();

		for (int i = 0; i < world->getNumEntities(); ++i) {
			std::shared_ptr<Camera> isCamera = std::dynamic_pointer_cast<Camera>(world->getEntity(i));

			if (!isCamera) {}
			else {
				camera->setEuler(glm::vec3(camera->getEuler().x - speedMY, camera->getEuler().y - speedMX, 0));

				if (up == GLFW_PRESS) {
					camera->move(glm::vec3(0, 0, -deltaTime * 10));
				}
				if (down == GLFW_PRESS) {
					camera->move(glm::vec3(0, 0, deltaTime * 10));
				}
				if (left == GLFW_PRESS) {
					camera->move(glm::vec3(-deltaTime * 10, 0, 0));
				}
				if (right == GLFW_PRESS) {
					camera->move(glm::vec3(deltaTime * 10, 0, 0));
				}
			}
		}

        camera->setProjection(glm::perspective(
            glm::radians(90.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.1f, 100.0f));
		camera->setViewport(glm::ivec4(0, 0, screenWidth, screenHeight));
		camera->prepare();

		world->draw();

        // Update swap chain & process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
