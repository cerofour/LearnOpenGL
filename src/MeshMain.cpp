#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <stb_image/stb_image.h>

#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <format>
#include <math.h>

#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include "FileReader.hpp"
#include "Texture.hpp"
#include "Types.hpp"
#include "scene/Model.hpp"

std::vector<float> cube_vertices{
#include "cube_vertices"
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

	auto& io = ImGui::GetIO();

	if (io.WantCaptureMouse) {
		// Enable the cursor when ImGui is capturing input
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

			std::cout << "[WIREFRAME MODE: ]" << context.getWireframeMode() << std::endl;

			context.getWireframeMode() ?
				glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON) :
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			context.setWireframeMode(!context.getWireframeMode());
		}
		else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			//std::cout << "[PAUSED]: " << context.paused << std::endl;
		}
		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			context.getCamera().goForward();
		}
		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			context.getCamera().goLeft();
		}
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			context.getCamera().goBackwards();
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			context.getCamera().goRight();
		} //else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//context.position.y -= 0.1f;
		//}
	}
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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
	glEnable(GL_DEPTH_TEST);

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

#pragma region Callbacks
	/*
	* Set the according viewport everytime the user resizes the window
	*/
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwMaximizeWindow(window);
	glfwSetCursorPosCallback(window, mouse_callback);
#pragma endregion

#pragma region Shader programs
	dlb::ShaderProgramBuilder spb{};
	spb
		.vertexShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\model_loading.vert")
		.fragmentShader("C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\shaders\\model_loading.frag");

	const auto block_shaders = spb.build();
#pragma endregion

#pragma region Scene Configuration
	glm::vec3 bg_color{ 0.0f };

	scene::Model backpack{ "C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\models\\backpack\\backpack.obj" };
	scene::Model merchi{ "C:\\Users\\Diego\\Documents\\Code\\LearnOpenGL\\resources\\models\\merchi.obj" };
#pragma endregion

	while (!glfwWindowShouldClose(window)) {
		proccessInput(window);
		context.updateTime();

		glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region ImGui Panel
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(300, 500));
		ImGui::Begin("Scene Configuration");

		ImGui::Text(std::format("FPS: {}/{}",
			context.getFrames(), 1000.0f / (float)context.getFrames()).c_str());

		ImGui::Text(std::format("X: {} Y: {} Z: {}",
			context.getCamera().getPosition().x, context.getCamera().getPosition().y, context.getCamera().getPosition().z).c_str());

		ImGui::Text(std::format("Looking at X: {} Y: {} Z: {}",
			context.getCamera().getDirection().x, context.getCamera().getDirection().y, context.getCamera().getDirection().z).c_str());

		ImGui::ColorEdit3("Bg Color", glm::value_ptr(bg_color)); // Adjust light source color
		ImGui::End();
#pragma endregion

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		backpack.draw(block_shaders);
		merchi.draw(block_shaders);

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

