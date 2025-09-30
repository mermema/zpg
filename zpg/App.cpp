#include "App.h"
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "ShapeBuilder.h"
#include "Sheders.h"

void App::createShaders() {

	blueShader.load(vertexShader, fragmentShaderBlue);
	redShader.load(vertexShader, fragmentShaderRed);
}

void App::createModels() {

	auto squareVerts = ShapeBuilder::makeSquare({ 0.3f, 0.3f, 1.0f }, { 0.9f, 0.9f, 1.0f });

	//auto cubeVerts = ShapeBuilder::makeCube({ -0.6f, -0.6f, -0.6f }, { -0.4f, -0.4f, -0.2f });

	squareModel.upload(squareVerts);


	triangleModel.upload({
		{-0.5f, -0.5f, 0.0f},
		{ 0.5f, -0.5f, 0.0f},
		{ 0.0f,  0.5f, 0.0f}
		});
}



static void error_callback(int error, const char* description) { fputs(description, stderr); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

void App::init() {

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	//inicializace konkretni verze
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //zakazani pouziti starych funkci
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);  //moderni pipeline povinne shadery

	this->window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window); //ted pouzivej toto okno
	glfwSwapInterval(1);// swap nastany kazdy 1 refresh framu na monitoru

	// init glew, glewExperimental povoluje pokrocile fce
	glewExperimental = GL_TRUE;
	glewInit();


	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);



	int width, height;
	glfwGetFramebufferSize(window, &width, &height);// vrati w a h pro nase okno
	float ratio = width / (float)height;//pomer stran
	glViewport(0, 0, width, height);//to co se vykresluje mapuj zde do tohoto obdelniku
}

void App::run() {

	while (!glfwWindowShouldClose(this->window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		blueShader.use();
		triangleModel.draw();
		redShader.use();
		squareModel.draw();
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	glfwDestroyWindow(this->window);
	glfwTerminate();
}
