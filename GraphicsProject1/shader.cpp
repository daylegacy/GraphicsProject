#include "shader.h"

std::string shader::loadShaderSource(const char * filename) {
	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	in_file.open(filename);

	//read file
	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else {
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << filename << "\n";
	}
	in_file.close();

	std::string version =
		std::to_string(this->versionMajor) +
		std::to_string(this->versionMinor) +
		"0";
	src.replace(src.find("#version"), 12, "#version " + version);
	return src;
}

GLuint shader::loadShader(GLenum type, const char * filename) {
	char infoLog[512];
	GLint success;

	GLuint shader = glCreateShader(type);
	std::string str_src = this->loadShaderSource(filename);
	const GLchar* src = str_src.c_str();
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_SHADER: " << filename << "\n";
		std::cout << infoLog << "\n";
	}
	return shader;
}

void shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
	char infoLog[512];
	GLint success;

	this->id = glCreateProgram();

	glAttachShader(this->id, vertexShader);
	//glUseProgram(this->id);

	if (geometryShader) {
		glAttachShader(this->id, geometryShader);
	}
	glAttachShader(this->id, fragmentShader);

	glLinkProgram(this->id);

	glGetShaderiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "ERROR::LINK_PROGRAM::COULD_NOT_LINK_PROGRAM\n";
		std::cout << infoLog << "\n";
	}
}

shader::shader(const int versionMajor, const int versionMinor, const char * vertexFile, const char * fragmentFile, const char* geometryFile)
	:versionMajor(versionMajor), versionMinor(versionMinor)
{
	
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

	vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
	if (geometryFile!="") {
		geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
	}
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

	this->linkProgram(vertexShader, geometryShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}


shader::~shader()
{
	glDeleteProgram(this->id);
}


void shader::use() {
	glUseProgram(this->id);
}

void shader::unuse() {
	glUseProgram(0);
}

void shader::set1i(GLint value, const GLchar* name) {
	this->use();

	glUniform1i(glGetUniformLocation(this->id, name), value);

	this->unuse();
}

void shader::set1f(GLfloat value, const GLchar* name) {
	this->use();

	glUniform1f(glGetUniformLocation(this->id, name), value);

	this->unuse();
}
void shader::setVec2f(glm::fvec2 value, const GLchar* name) {
	this->use();

	glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}
void shader::setVec3f(glm::fvec3 value, const GLchar* name) {
	this->use();

	glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

void shader::setVec4f(glm::fvec3 value, const GLchar* name) {
	this->use();

	glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

	this->unuse();
}

void shader::setMat3fv(glm::mat4 value, const GLchar* name, GLboolean transpose) {
	this->use();

	glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

	this->unuse();
}

void shader::setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose) {
	this->use();

	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));

	this->unuse();
}