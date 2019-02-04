#pragma once 

#include <iostream>
#include<fstream>
#include<string>

#include <glew.h>
#include <glfw3.h>

#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>

#include<SOIL2.h>
class Shader
{
private:

	//member variables
	GLuint id;
	const int versionMajor;
	const int versionMinor;
	//private functions
	std::string loadShaderSource(const char * filename);
	GLuint loadShader(GLenum type, const char * filename);
	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
public:
	//constructors/destructors
	Shader(const int versionMajor, const int versionMinor, const char * vertexFile, const char * fragmentFile, const char* geometryFile="");
	void use();
	void unuse();
	~Shader();
	void set1i(GLint value, const GLchar* name);
	
	void set1f(GLfloat value, const GLchar* name);
	void setVec2f(glm::fvec2 value, const GLchar* name);
	void setVec3f(glm::fvec3 value, const GLchar* name);

	void setVec4f(glm::fvec3 value, const GLchar* name);

	void setMat3fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);
};

