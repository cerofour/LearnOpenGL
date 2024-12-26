#pragma once

#include <string>

#include "Camera.hpp"

namespace dlb {
	class ApplicationSingleton {

	private:
		ApplicationSingleton()
			:camera{glm::vec3(0.0f, 0.0f, -5.0f)}
		{
			window_dims.x = 800;
			window_dims.y = 600;

			last_cursor_pos = { window_dims.x / 2.0f, window_dims.y / 2.0f };

			window_title = "Learn OpenGL";
			delta_time = 0.0f;
			last_frame = 0.0f;

			wireframe_mode = false;
		}

	public:
		static ApplicationSingleton& getInstance() {
			static ApplicationSingleton instance{};
			return instance;
		}

		inline const glm::vec2& getWindowDims() const {
			return window_dims;
		}

		inline const std::string& getWindowTitle() const {
			return window_title;
		}

		inline double getDeltaTime() const {
			return delta_time;
		}

		inline double getLastFrame() const {
			return last_frame;
		}

		inline void setWindowDimsW(float value) {
			window_dims.x = value;
		}

		inline void setWindowDimsH(float value) {
			window_dims.y = value;
		}

		inline dlb::Camera& getCamera() {
			return camera;
		}

		inline bool getWireframeMode() {
			return wireframe_mode;
		}

		inline void setWireframeMode(bool val) {
			wireframe_mode = val;
		}

		inline glm::vec2& getLastCursor() {
			return last_cursor_pos;
		}

		void setLastCursorX(float val) {
			last_cursor_pos.x = val;
		}

		void setLastCursorY(float val) {
			last_cursor_pos.y = val;
		}

		int getFrames() {
			return frames;
		}

		void updateTime();

	private:
		glm::vec2 window_dims;
		glm::vec2 last_cursor_pos;
		std::string window_title;

		double delta_time;
		double last_frame;
		double last_time = 0.0F;

		dlb::Camera camera;

		bool wireframe_mode;

		int frames = 0;
	};
}