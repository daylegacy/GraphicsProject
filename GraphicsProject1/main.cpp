#include "libs.h"
#include "libs.h"

void framebuffer_resize_callback(GLFWwindow * window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}


bool loadShaders(GLuint &program) {
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//vertex
	in_file.open("vertex_core.glsl");

	//read file
	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else {
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE\n";
		loadSuccess = false;
	}
	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER\n";
		std::cout << infoLog << "\n";
		in_file.close();
	}

	temp = "";
	src = "";

	//Fragment
	in_file.open("fragment_core.glsl");

	//read file
	if (in_file.is_open()) {
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else {
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE\n";
		in_file.close();
	}
	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER\n";
		std::cout << infoLog << "\n";
		in_file.close();
	}

	//Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM\n";
		std::cout << infoLog << "\n";
		in_file.close();
	}

	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
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

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "window", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight); 
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);


	//INIT GLEW(NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//shader init
	GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();


	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		//UPDATE INPUT
		glfwPollEvents();

		//update

		//Draw
		//Clear
		glClearColor(0.f, 0.5f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//Draw

		//End of draw
		glfwSwapBuffers(window);
		glFlush();
	}
	//END OF PROGRAM
	glfwDestroyWindow(window);

	glfwTerminate();
	//delete program
	glDeleteProgram(core_program);
	return 0;
}