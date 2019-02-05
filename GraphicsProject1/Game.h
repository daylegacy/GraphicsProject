#pragma once
#include <iostream>
#include<fstream>
#include<string>
#include<vector>

#include"libs.h"

enum shader_enum { SHADER_CORE_PROGRAM = 0 };
enum texture_enum { TEX_CRATE0 = 0, TEX_CRATE_SPECULAR1 };
enum material_enum { MAT_1 = 0 };
enum mesh_enum { MESH_QUAD =0};
 

class Game
{
private:
	//Variables
	GLFWwindow * window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	float fov;
	float nearPlane;
	float farPlane;

	//Vectors
	std::vector<Shader*>	shaders;
	std::vector<Texture*>	textures;
	std::vector<Material*>	materials;
	std::vector<Mesh*>		meshes;
	std::vector<glm::vec3*>	lights;


	//Private functions
	void initGLFW();
	void initWindow(const char * title,
		bool resizable);
	void initGLEW();
	void initOpenGLOptions();

	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();

	void updateUniforms();

	static void framebuffer_resize_callback(GLFWwindow * window, int fbw, int fbh);
public:
	//Constructors/Destructor
	Game(const char * title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		int GLmajorVer, int GLminorVer,
		bool resizable);

	virtual ~Game();

	//Accessors
	inline GLFWwindow* getWindow() {return window;}
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	//Functions
	void update();
	void render();

	//static functions
	static void updateInput(GLFWwindow * window, Mesh & mesh);
	static void updateInput(GLFWwindow * window);

};

