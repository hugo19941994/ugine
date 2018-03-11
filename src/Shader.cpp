#include "Shader.h"

Shader::Shader(std::string vertexShader, std::string fragmentShader) {
	if (vertexShader == "" || fragmentShader == "") {
		std::cout << "could not load shaders" << std::endl;
	}

	// Create Vertex Shader
	int retCode = GL_FALSE;
	Shader::vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderC = vertexShader.c_str();
	glShaderSource(vs, 1, &vertexShaderC, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vs, sizeof(Shader::errorLog), nullptr, Shader::errorLog);
		std::cout << Shader::errorLog << std::endl;
	}

	// Create Fragment Shader
	Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderC = fragmentShader.c_str();
	glShaderSource(fs, 1, &fragmentShaderC, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fs, sizeof(Shader::errorLog), nullptr, Shader::errorLog);
		std::cout << Shader::errorLog << std::endl;
	}

	// Create Program
	Shader::program = glCreateProgram();
	glAttachShader(Shader::program, vs);
	glAttachShader(Shader::program, fs);
	glLinkProgram(Shader::program);
	glGetProgramiv(Shader::program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(Shader::program, sizeof(Shader::errorLog), nullptr, Shader::errorLog);
		std::cout << Shader::errorLog << std::endl;
	}

	// TODO: Hacer generico?
	Shader::vposLoc = glGetAttribLocation(Shader::program, "vpos");
}
Shader::~Shader()
{
	glDeleteShader(Shader::vs);
	glDeleteShader(Shader::fs);
}

uint32_t Shader::getId() const
{
	return Shader::program;
}

const char * Shader::getError() const
{
	return Shader::errorLog;
}

void Shader::use() const
{
	// Use Program
	glUseProgram(Shader::program);
}

void Shader::setupAttribs() const
{
	if (Shader::vposLoc != -1) {
		glEnableVertexAttribArray(Shader::vposLoc);
		glVertexAttribPointer(vposLoc, 3, GL_FLOAT, false, sizeof(float) * 3, 0);
	}
	else {
		std::cout << "setupAttribs failed" << std::endl;
	}
}

int Shader::getLocation(const char * name) const
{
	return glGetUniformLocation(Shader::program, name);
}

void Shader::setInt(int loc, int val)
{
	if (loc != -1) {
		glUniform1i(loc, val);
	}
	else {
		std::cout << "setInt failed" << std::endl;
	}
}

void Shader::setFloat(int loc, float val)
{
	if (loc != -1) {
		glUniform1f(loc, val);
	}
	else {
		std::cout << "setFloat failed" << std::endl;
	}
}

void Shader::setVec3(int loc, const glm::vec3 & vec)
{
	if (loc != -1) {
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
	else {
		std::cout << "setVec3 failed" << std::endl;
	}
}

void Shader::setVec4(int loc, const glm::vec4 & vec)
{
	if (loc != -1) {
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}
	else {
		std::cout << "setVec4 failed" << std::endl;
	}
}

void Shader::setMatrix(int loc, const glm::mat4 & matrix)
{
	if (loc != -1) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	} else {
		std::cout << "setMatrix failed" << std::endl;
	}
}