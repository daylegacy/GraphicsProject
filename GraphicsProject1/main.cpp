#include "libs.h"


Vertex vertices[] = {
	//Position							//color						//texcoords				//normals
	
	glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f,-0.5f, 0.f),			glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, 1.f),
	glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, 1.f)
};
unsigned nrOfVertices = sizeof(vertices)/sizeof Vertex;

GLuint indices[] =
{
	0, 1, 2, //triangle 1
	0, 2, 3 //tringle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);


void framebuffer_resize_callback(GLFWwindow * window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

void updateInput(GLFWwindow  *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}




void updateInput(GLFWwindow * window, Mesh & mesh) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
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
	}
}

int main() {
	//INIT GLFW
	glfwInit();

	//CREATE WINDOW
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwSwapInterval(6);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);

	//important with perspective matrix
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);


	//INIT GLEW(NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//open_gl options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 



	//shader init
	shader core_program("vertex_core.glsl", "fragment_core.glsl");
	//model

	Mesh test(vertices, nrOfVertices, indices, nrOfIndices,
		glm::vec3(0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f)
	);
	
	//texture0 init
	Texture texture_0("Images/crate.png", GL_TEXTURE_2D ,0);

	//texture1 init
	Texture texture_1("Images/cat.png", GL_TEXTURE_2D, 1);

	//material
	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(4.f), texture_0.getTextureUnit(), texture_1.getTextureUnit());
	

	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 120.f;
	float nearPlane = 0.1f;
	float farPlane = 1000.f;

	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);

	//Lights
	glm::vec3 lightPos0(0.f, 0.f, 1.f);

	//INIT uniforms
	core_program.use();
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");


	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//UPDATE INPUT
		glfwPollEvents();
		updateInput(window, test);

		//update
		updateInput(window);
		//draw
		//clear
		glClearColor(0.f, 0.2f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		//restrict shortening depends on window proportions but cant minimize
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		//projection matrix
		glm::mat4 ProjectionMatrix(1.f);
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//view matrix
		ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
		core_program.setMat4fv(ViewMatrix, "ViewMatrix");
		//Use a program
		
		texture_0.bind();
		texture_1.bind();
		material0.sendToShader(core_program);

		//Draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		test.render(&core_program);

		//End of draw
		glfwSwapBuffers(window);
		glFlush();

		glUseProgram(0);
		texture_0.unbind();
		texture_1.unbind();
	}
	//END OF PROGRAM
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}