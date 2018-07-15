#pragma once // NOLINT
#include "../lib/glew/glew.h"
#include "../lib/glfw3/glfw3.h"
#include "common.h"
#include "Vertex.h"
#include "string"
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

class Shader {
  private:
    // Program Id
    uint32_t program;

    // Error Log
    char errorLog[1024];

    int vposLoc;
    int vtexLoc;
    int vnormalLoc;
    int vtangentLoc;

    // Shader locations
    uint32_t vs;
    uint32_t fs;

  public:
    Shader(std::string const &vertexShader, std::string const &fragmentShader);
    ~Shader();

    // Devuelve el identificador de OpenGL del programa
    uint32_t getId() const;

    // Obtiene el mensaje de error generado al compilar o enlazar
    const char *getError() const;

    // Activa el uso de este programa
    void use() const;

    // Activa la escritura de las variables attribute,
    // y especifica su formato
    void setupAttribs() const;

    // Obtiene la localización de una variable uniform
    GLint getLocation(const char *name) const;

    // Da valor a una variable uniform
    void setInt(int loc, int val);
    void setFloat(int loc, float val);
    void setVec3(int loc, const glm::vec3 &vec);
    void setVec4(int loc, const glm::vec4 &vec);
    void setMatrix(int loc, const glm::mat4 &matrix);
};
