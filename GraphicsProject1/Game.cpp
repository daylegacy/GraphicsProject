#include "Game.h"



void Game::initGLFW()
{
	if (glfwInit()==GLFW_FALSE) {
		std::cout << "ERROR::GLFW_INIT::FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(const char * title,
	bool resizable)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT,
		"window", NULL, NULL);
	if (this->window == nullptr) {
		std::cout << "ERROR::GLFW_CREATE_WINDOW::FAILED" << "\n";
		glfwTerminate();
	}

	//important with perspective matrix
	glfwGetFramebufferSize(window, &this->framebufferWidth, &this->framebufferHeight);
	if (resizable)  glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);
}

void Game::initGLEW()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	//open_gl options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	//texture 0
	this->textures.push_back(new Texture("Images/crate.png", GL_TEXTURE_2D));
	//texture1 init
	this->textures.push_back(new Texture("Images/crate_specular.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Game::initMeshes()
{
	this->meshes.push_back(
		new Mesh(&Pyramid(),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);
}

void Game::initLights()
{
	this->lights.push_back(new glm::vec3(0.f, 4.f, 1.f));
}

void Game::initUniforms()
{
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");

	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
}

void Game::updateUniforms()
{
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
	
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	/*this->ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);*/
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	//view matrix
	this->ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
}

inline void Game::framebuffer_resize_callback(GLFWwindow * window, int fbw, int fbh) {
	glViewport(0, 0, fbw, fbh);
}

Game::Game(const char * title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GLVerMajor, const int GLVerMinor,
	bool resizable)
	: WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GLVerMajor), GL_VERSION_MINOR(GLVerMinor)
{
	this->window = nullptr;
	this->framebufferHeight = WINDOW_HEIGHT;
	this->framebufferWidth = WINDOW_WIDTH;
	

	//init matrices
	this->camPosition=glm::vec3(0.f, 0.f, 1.f);
	this->worldUp= glm::vec3(0.f, 1.f, 0.f);
	this->camFront= glm::vec3(0.f, 0.f, -1.f);
	this->fov = 65.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initLights();
	this->initMeshes();

}


Game::~Game()
{
	
	glfwDestroyWindow(this->window);
	glfwTerminate();
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete shaders[i];
	for (size_t i = 0; i < this->textures.size(); i++)
		delete textures[i];
	for (size_t i = 0; i < this->meshes.size(); i++)
		delete meshes[i];
	for (size_t i = 0; i < this->materials.size(); i++)
		delete materials[i];
	for (size_t i = 0; i < this->lights.size(); i++)
		delete lights[i];
}

int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::update()
{
	this->updateInput();
	//this->updateInput(this->window, *meshes[MESH_QUAD]);
}

void Game::render()
{
	glClearColor(0.f, 0.2f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//update uniforms
	this->updateUniforms();
	this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	//Use a program
	this->shaders[SHADER_CORE_PROGRAM]->use();

	this->textures[TEX_CRATE0]->bind(0);
	this->textures[TEX_CRATE_SPECULAR1]->bind(1);

	//Draw
	//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
	this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);

	//End of draw
	glfwSwapBuffers(window);
	glFlush();

	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Game::updateInput() {
	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
}

void Game::updateKeyboardInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->camPosition.z -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->camPosition.z += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->camPosition.x -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->camPosition.x += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS) {
		this->camPosition.y += 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->camPosition.y -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS) {
		glm::mat3 rotation(0.f);
		rotation[0][0] = rotation[2][2] = glm::cos(0.1);
		rotation[0][2] = -glm::sin(0.1);
		rotation[2][0] = glm::sin(0.1);
		rotation[1][1] = 1;

		this->camFront = rotation * this->camFront;
	}
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) {
		glm::mat3 rotation(0.f);
		rotation[0][0] = rotation[2][2] = glm::cos(-0.1);
		rotation[0][2] = -glm::sin(-0.1);
		rotation[2][0] = glm::sin(-0.1);
		rotation[1][1] = 1;

		this->camFront = rotation * this->camFront;
	}
}

void Game::updateMouseInput()
{
}



void Game::updateInput(GLFWwindow * window, Mesh & mesh) {
	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.f, 0.f, -0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.f, 0.f, 0.01f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		mesh.move(glm::vec3(0.01f, 0.f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.f, -0.01f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		mesh.rotate(glm::vec3(0.f, 0.01f, 0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		mesh.scale(glm::vec3(-0.1f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		mesh.scale(glm::vec3(0.1f));
	}*/
}