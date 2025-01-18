#include <functional>

#include "Application.hpp"

#include <GLFW/glfw3.h>

static void APIENTRY glDebugOutput(GLenum source,
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

namespace dlb {
	GLFWwindow* createWindow(int w, int h, const char* title) {
		GLFWwindow* window = glfwCreateWindow(
			w,
			h,
			title,
			NULL,
			NULL);

		return window;
	}

	void failOnCondition(bool cond, const std::function<void()>& cleanup) {
		if (cond) {
			cleanup();
			exit(-1);
		}
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		auto& context = dlb::ApplicationSingleton::getInstance();

		context.setWindowDimsH(height);
		context.setWindowDimsW(width);
		glViewport(0, 0, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

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

	void ApplicationSingleton::init() {

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

		window_ = createWindow(window_dims.x, window_dims.y, window_title.c_str());
		failOnCondition(window_ == NULL, [] {glfwTerminate(); });

		glfwMakeContextCurrent(window_);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(-1);
		}

		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glViewport(0, 0, getWindowDims().x, getWindowDims().y);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		/*
		* Set the according viewport everytime the user resizes the window
		*/
		glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
		std::cout << "init1 done\n";

		//glfwMaximizeWindow(window_);
		std::cout << "init2 done\n";

		glfwSetCursorPosCallback(window_, mouse_callback);

		std::cout << "init3 done\n";
	}
	void ApplicationSingleton::updateTime() {
		float current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;

		frames++;

		if (current_time - last_time >= 1.0F) {
			frames = 0;
			last_time += 1.0f;
		}
	}
}