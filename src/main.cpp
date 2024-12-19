#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <format>
#include <math.h>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include "Renderable.hpp"

std::vector<float> vertices = {
	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f,  0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,

	-0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,

	-0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f,  0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,

	 0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f,  0.5f,  0.5f, 0.58f, 0.8f, 0.92f,

	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	 0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f,  0.5f, 0.58f, 0.8f, 0.92f,
	-0.5f, -0.5f, -0.5f, 0.58f, 0.8f, 0.92f,

	-0.5f,  0.5f, -0.5f, 0.58f, 0.8, 0.92,
	 0.5f,  0.5f, -0.5f, 0.58f, 0.8, 0.92,
	 0.5f,  0.5f,  0.5f, 0.58f, 0.8, 0.92,
	 0.5f,  0.5f,  0.5f, 0.58f, 0.8, 0.92,
	-0.5f,  0.5f,  0.5f, 0.58f, 0.8, 0.92,
	-0.5f,  0.5f, -0.5f, 0.58f, 0.8, 0.92,
};

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

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

#pragma region Callbacks
	/*
	* Set the according viewport everytime the user resizes the window
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
#pragma endregion

#pragma region Shader programs
	dlb::ShaderProgramBuilder spb{};

	spb
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\light.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\light.frag");

	const auto block_shaders = spb.build();

	const auto light_source_shaders = dlb::ShaderProgramBuilder()
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\light_source.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\light_source.frag")
		.build();
#pragma endregion

#pragma region Creating renderable objects

	dlb::Renderable light_source;
	glm::vec3 light_source_position{ 7.3f, 0.0f, -10.0f};
	glm::vec3 light_source_color{ 1.0f, 0.0f, 0.0f };
	light_source.setShaderProgram(&light_source_shaders);
	light_source.feedData(std::move(std::vector<float>(vertices)), 36);
	light_source.configureVertexAttributes(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		[] {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), NULL);
			glEnableVertexAttribArray(0);
			//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), (void*)(3 * sizeof(float)));
			//glEnableVertexAttribArray(1);
		});


	dlb::Renderable cube;
	cube.setShaderProgram(&block_shaders);
	cube.feedData(std::move(std::vector<float>(vertices)), 36);
	cube.configureVertexAttributes(GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		[] {
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), NULL);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(vertices[0]), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		});
#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)context.getWindowDims().x / (float)context.getWindowDims().y, 0.1f, 100.0f);

		proccessInput(window);

		std::cout << std::format("[CAMERA]: {} {} {}\n",
			context.getCamera().getDirection().x,
			context.getCamera().getDirection().y,
			context.getCamera().getDirection().z);

		context.updateTime();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region ImGui Panel
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Light Configuration");
		ImGui::ColorEdit3("Light Color", glm::value_ptr(light_source_color)); // Adjust light source color
		ImGui::End();
#pragma endregion

		light_source.render(
			[&](const dlb::ShaderProgram* sp) {
				sp->use();
				sp->setUniform("u_light_color", light_source_color);
				const glm::mat4 model = glm::translate(glm::mat4(1.0F), light_source_position);
				const glm::mat4 view = context.getCamera().getView();

				sp->setUniform("u_model", model);
				sp->setUniform("u_view", view);
				sp->setUniform("u_projection", projection);
			}
		);

		cube.render(
			[&](const dlb::ShaderProgram* sp) {
				sp->use();
				sp->setUniform("u_light_color", light_source_color);

				const glm::mat4 model = glm::translate(glm::mat4(1.0F), glm::vec3(5.3f, 0.0f, -6.0f));
				const glm::mat4 view = context.getCamera().getView();

				sp->setUniform("model", model);
				sp->setUniform("view", view);
				sp->setUniform("projection", projection);
			}
		);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

