#pragma once

#include <glm/glm.hpp>

namespace dlb {
	class Camera {

	public:
		Camera(const glm::vec3& start_position);


	public:
		const glm::mat4& getView();

	public:
		inline const glm::vec3& getPosition() const { return position; }
		inline const glm::vec3& getDirection() const { return direction; }
		inline const glm::vec3& getRight() const { return right; }
		inline const glm::vec3& getUp() const { return up; }

		void goForward();
		void goBackwards();
		void goRight();
		void goLeft();

		void updateDirection(const glm::vec2& offsets);

		float& getCameraSpeedRef() {
			return camera_speed;
		}

	private:
		float getCameraSpeed();
		void updateView();

	private:
		glm::vec3 position,
			direction,
			right,
			up;

		glm::mat4 view;

		float camera_speed = 3.0f;
		float yaw = -90.0f;
		float pitch = 0.0f;
};
}