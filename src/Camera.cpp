#include <glad/glad.h>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

#include "Camera.hpp"

#include "Application.hpp"

namespace dlb {

	Camera::Camera(const glm::vec3& start_position) {

		const glm::vec3 origin{ 0.0F };
		const glm::vec3 world_up{ 0.0f, 1.0f, 0.0f };

		position = start_position;
		direction = glm::normalize(position - origin);
		right = glm::normalize(
			glm::cross(world_up, direction));
		up = glm::cross(direction, right);

		updateView();
	}

	const glm::mat4& Camera::getView() {
		updateView();

		return view;
	}

	void Camera::updateView() {
		view = glm::lookAt(
			position,
			direction + position,
			up
		);
	}

	void Camera::updateDirection(const glm::vec2& offsets) {
		yaw += offsets.x;
		pitch += offsets.y;

		if (pitch >= 89.0f)
			pitch = 89.0f;
		if (pitch <= -89.0f)
			pitch = -89.0f;

		direction = glm::normalize(
			glm::vec3(
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
				));
	}

	float Camera::getCameraSpeed() {
		return camera_speed * dlb::ApplicationSingleton::getInstance().getDeltaTime();
	}

	void Camera::goForward() {
		position += getCameraSpeed() * direction;
	}

	void Camera::goBackwards() {
		position -= getCameraSpeed() * direction;
	}

	void Camera::goRight() {
		position += glm::normalize(glm::cross(direction, up)) * getCameraSpeed();
	}

	void Camera::goLeft() {
		position -= glm::normalize(glm::cross(direction, up)) * getCameraSpeed();
	}


}