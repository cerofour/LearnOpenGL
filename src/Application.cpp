#include <GLFW/glfw3.h>

#include "Application.hpp"

namespace dlb {
	void ApplicationSingleton::updateTime() {
		float current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;
	}
}