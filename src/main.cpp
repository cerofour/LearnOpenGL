#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <functional>
#include <math.h>

#include "ShaderProgram.hpp"

struct ApplicationContext {
	glm::ivec2 window_dims = { 800, 600 };
	const std::string window_title = "LearnOGL";
	bool wireframe_mode = false;
} context;

void failOnCondition(bool cond, const std::function<void()>& cleanup) {
	if (cond) {
		cleanup();
		exit(-1);
	}
}

GLFWwindow* createWindow() {
	GLFWwindow* window = glfwCreateWindow(
		context.window_dims.x,
		context.window_dims.y,
		context.window_title.c_str(),
		NULL,
		NULL);

	return window;
}

void proccessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

		std::cout << "[WIREFRAME MODE: ]" << context.wireframe_mode << std::endl;

		context.wireframe_mode ?
			glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON) :
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		context.wireframe_mode = !context.wireframe_mode;
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	auto window = createWindow();
	failOnCondition(window == NULL, [] {glfwTerminate(); });

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, context.window_dims.x, context.window_dims.y);

#pragma region Callbacks
	/*
	* Set the according viewport everytime the user resizes the window
	*/
	glfwSetFramebufferSizeCallback(window,
		[](GLFWwindow* win, int w, int h) {
			context.window_dims.x = w;
			context.window_dims.y = h;
			std::cout << "Resized to: " << w << "X" << h << std::endl;
			glViewport(0, 0, w, h);
		});

	/*
	* Set the window color according to the cursor position
	*/
	glfwSetCursorPosCallback(window,
		[](GLFWwindow* win, double x, double y) {
			std::cout << "[CURSOR]: (" << x << ";" << y << ")" << std::endl;
			float red = static_cast<float>(x) / context.window_dims.x;
			float blue = static_cast<float>(y) / context.window_dims.y;
			float green = 1.0f;

			glClearColor(red, green, blue, 1.0f);
		});
#pragma endregion

#pragma region Shader program

	dlb::ShaderProgramBuilder spb{};

	spb.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\cmakeSetup-102a06694aee21c1c2bd32722fd9bcd1c488f2d3\\resources\\fragment.frag")
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\cmakeSetup-102a06694aee21c1c2bd32722fd9bcd1c488f2d3\\resources\\vertex.vert");

	unsigned int shaderProgram = spb.build();
#pragma endregion

#pragma region Feeding data to the GPU

	glm::float32 vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	glm::uint indices[] = {
		0, 1, 2,
	};

	glm::uint VAO;
	glGenVertexArrays(1, &VAO);

	// Now this VAO will store all the following VBO configuration
	glBindVertexArray(VAO);

	glm::uint EBO;
	glGenBuffers(1, &EBO);

	glm::uint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/*
	* Tell OpenGL how to understand the data un the vertex buffer
	* - The first argument is the attribute we want to configure (0), look at the vertex shader were we
	* configured aPos to have location 0, this attribute will be mapped to aPos.
	* - The second argument is the amount of components this attribute has = 3
	* - The third is the type of the argument (Float)
	* - the fourth argument tells if we want openGL to normalize the data.
	* - the fifth is the total size of the vertex (3 floats)
	* - the fourth is the offset (0) in this case because this attribute is just at the start.
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(vertices[0]), NULL);
	glEnableVertexAttribArray(0);
#pragma endregion

	while (!glfwWindowShouldClose(window)) {

		proccessInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

#pragma region Make the GPU draw a triangle
		glUseProgram(shaderProgram);


#pragma region Uniform Manipulation

		float green_val = sin(glfwGetTime());
		float red_val = cos(glfwGetTime());
		float blue_val = 0.5f;

		int variating_color_location = glGetUniformLocation(shaderProgram, "variatingColor");
		glUniform3f(variating_color_location, red_val, green_val, blue_val);

#pragma endregion

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}