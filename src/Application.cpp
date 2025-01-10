#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace dlb {
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