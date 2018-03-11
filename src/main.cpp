#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "Buffer.h"
#include "Shader.h"
#include "Vertex.h"
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

int main() {
    // Init GLEW
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "could not initalize glfw" << std::endl;
        return -1;
    }

    // Create Window
    GLFWwindow *window = glfwCreateWindow(800, 600, "U-gine",
                                          FULLSCREEN ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "could not create glfw window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Init GLEW
    if (glewInit() != 0) {
        std::cout << "could not initialize glew" << std::endl;
        return -1;
    }

    // Enable OpenGL States
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    atexit(glfwTerminate);

    // Read Shaders
    std::string vertexShader = readString("data/shader.vert");
    std::string fragmentShader = readString("data/shader.frag");
    Shader shader(vertexShader, fragmentShader);

    shader.use();

    // Triangles
    std::vector<Vertex> vertices = {Vertex(0.f, 1.f, 0.f), Vertex(-1.f, -1.f, 0.f),
                                    Vertex(1.f, -1.f, 0.f)};
    std::vector<int> indices = {0, 1, 2};
    Buffer buffer(vertices, indices);

    // Camera matrix (part of MVP)
    glm::mat4 View =
        glm::lookAt(glm::vec3(0, 0, 6), // Camera is at (0, 0, 6), in World Space
                    glm::vec3(0, 0, 0), // and looks at the origin
                    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // MVP location
    GLint loc = shader.getLocation("MVP");

    auto lastTime = static_cast<float>(glfwGetTime());
    float accumulatedTime = 0;
    while (glfwWindowShouldClose(window) == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == 0) {
        // Update delta time
        auto newTime = static_cast<float>(glfwGetTime());
        float deltaTime = newTime - lastTime;
        accumulatedTime += deltaTime;
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
        glm::mat4 Projection = glm::perspective(
            glm::radians(90.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            0.1f, 100.0f);

        // Clear screen
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);
        glScissor(0, 0, frameBufferWidth, frameBufferHeight);
        glClearColor(0.2f, 0.2f, 0.2f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

        // Current rotation
        glm::mat4 Rotation =
            glm::rotate(glm::mat4(1.0f), glm::radians(32.0f) * accumulatedTime, glm::vec3(0, 1, 0));

        // x axis translation
        for (int i = -3; i < 6; i += 3) {
            // z axis translation
            for (int j = 0; j >= -6; j -= 3) {
                glm::mat4 Translation = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0, j));

                // Model matrix
                glm::mat4 Model = Translation * Rotation;

                glm::mat4 MVP = Projection * View * Model; // Remember : inverted !
                shader.setMatrix(loc, MVP);

                // Draw triangle
                buffer.draw(shader);
            }
        }

        // Update swap chain & process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
