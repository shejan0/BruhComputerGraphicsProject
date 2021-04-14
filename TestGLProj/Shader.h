#ifndef SHADER_H
#define SHADER_H
/*
 * Shader.h
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: Copied from Assignment 2, initializes and creates Shader programs in OpenGL context
 *
 *
 *
 *
 */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "Error.h"

class Shader {

public:
	Shader();
	~Shader();

	bool InitializeFromFile(const char* vertFilename, const char* fragFilename);

	bool SetUniform(std::string name, int value);
	bool SetUniform(std::string name, float value);
	bool SetUniform(std::string name, glm::vec3 value);
	bool SetUniform(std::string name, glm::vec4 value);
	bool SetUniform(std::string name, glm::mat3 value);
	bool SetUniform(std::string name, glm::mat4 value);

	bool AddAttribute(std::string attribute); // I Got rid of this because it was not needed. I wrapped this into the [] operator
	GLuint operator[](std::string attribute);

	int GetProgramID();
	bool isInitialized();
	void Activate();
	void DeActivate();

private:
	bool CreateShader();
	int LoadShaderFromFile(const char* filename, GLenum shaderType);

private:
	int m_vertex;
	int m_fragment;
	int m_program=-1;
	std::map<std::string, GLuint> m_attributeList;
};

#endif