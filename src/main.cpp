#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"

void failOnCondition(bool cond, const std::function<void()>& cleanup) {
	if (cond) {
		cleanup();
		exit(-1);
	}
}

GLFWwindow* createWindow() {

	auto& context = dlb::ApplicationSingleton::getInstance();
	auto& window_dims = context.getWindowDims();

	GLFWwindow* window = glfwCreateWindow(
		window_dims.x,
		window_dims.y,
		context.getWindowTitle().c_str(),
		NULL,
		NULL);

	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	dlb::ApplicationSingleton::getInstance().setWindowDimsH(height);
	dlb::ApplicationSingleton::getInstance().setWindowDimsW(width);
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

	auto& context = dlb::ApplicationSingleton::getInstance();

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	float xoffset = xpos - context.getLastCursor().x;
	float yoffset = context.getLastCursor().y - ypos; // reversed since y-coordinates go from bottom to top
	context.setLastCursorX(xpos);
	context.setLastCursorY(ypos);

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	context.getCamera().updateDirection(glm::vec2(xoffset, yoffset));
}

void proccessInput(GLFWwindow* window) {

	auto& context = dlb::ApplicationSingleton::getInstance();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	} else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

		std::cout << "[WIREFRAME MODE: ]" << context.getWireframeMode() << std::endl;

		context.getWireframeMode() ?
			glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON) :
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		context.setWireframeMode(!context.getWireframeMode());
	} else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		//std::cout << "[PAUSED]: " << context.paused << std::endl;
	} else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		context.getCamera().goForward();
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		context.getCamera().goLeft();
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		context.getCamera().goBackwards();
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		context.getCamera().goRight();
	} //else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		//context.position.y -= 0.1f;
	//}
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

	auto& context = dlb::ApplicationSingleton::getInstance();

	glViewport(0, 0, context.getWindowDims().x, context.getWindowDims().y);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

#pragma region Callbacks
	/*
	* Set the according viewport everytime the user resizes the window
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
#pragma endregion

#pragma region Shader program

	dlb::ShaderProgramBuilder spb{};

	spb
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\vertex.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\fragment.frag");

	unsigned int shaderProgram = spb.build();
#pragma endregion

#pragma region Feeding data to the GPU

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	std::vector<glm::vec3> cube_positions{
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::uint VAO;
	glGenVertexArrays(1, &VAO);

	// Now this VAO will store all the following VBO configuration
	glBindVertexArray(VAO);

	glm::uint VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

		context.updateTime();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region Make the GPU draw a triangle
		glUseProgram(shaderProgram);

#pragma region Uniform Manipulation

#pragma region Coordinate systems

		const glm::mat4& view = context.getCamera().getView();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)context.getWindowDims().x / (float)context.getWindowDims().y, 0.1f, 100.0f);
#pragma endregion

		int model_location = glGetUniformLocation(shaderProgram, "model");
		int view_location = glGetUniformLocation(shaderProgram, "view");
		int projection_location = glGetUniformLocation(shaderProgram, "projection");

		glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);

#pragma endregion

		glBindVertexArray(VAO);

#pragma region Iteratively draw various cubes with diferent positions

		for (const auto& pos : cube_positions) {
			//glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(-55.0F), pos);
			glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
			glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

#pragma endregion
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

